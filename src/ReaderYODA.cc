// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ReaderYODA.h"
#include "YODA/Utils/StringUtils.h"
#include "YODA/Exceptions.h"

#include "YODA/Counter.h"
#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Profile2D.h"
#include "YODA/Scatter1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Scatter3D.h"

#include <iostream>
using namespace std;

namespace YODA {

  // Static initializations
  qi::symbols<char, int> ReaderYODA::bgroup;
  qi::symbols<char, int> ReaderYODA::egroup;
  ReaderYODA::counter ReaderYODA::_counter;
  ReaderYODA::histo1d ReaderYODA::_histo1d;
  ReaderYODA::histo2d ReaderYODA::_histo2d;
  ReaderYODA::profile2d ReaderYODA::_profile2d;
  ReaderYODA::profile1d ReaderYODA::_profile1d;
  ReaderYODA::scatter1d ReaderYODA::_scatter1d;
  ReaderYODA::scatter2d ReaderYODA::_scatter2d;
  ReaderYODA::scatter3d ReaderYODA::_scatter3d;
  map<string, string> ReaderYODA::_annotations;


  namespace {

    /// @todo Move to Utils?
    // Portable version of getline taken from
    // http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    istream& safe_getline(istream& is, string& t) {
      t.clear();

      // The characters in the stream are read one-by-one using a std::streambuf.
      // That is faster than reading them one-by-one using the std::istream.
      // Code that uses streambuf this way must be guarded by a sentry object.
      // The sentry object performs various tasks,
      // such as thread synchronization and updating the stream state.
      istream::sentry se(is, true);
      streambuf* sb = is.rdbuf();

      for (;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
          return is;
        case '\r':
          if (sb->sgetc() == '\n')
            sb->sbumpc();
          return is;
        case EOF:
          // Also handle the case when the last line has no line ending
          if (t.empty())
            is.setstate(ios::eofbit);
          return is;
        default:
          t += (char)c;
        }
      }
    }

  }


  void ReaderYODA::_readDoc(istream& stream, vector<AnalysisObject*>& aos) {

    // These are the context groups we know. We need
    // that map to dynamically change the parser depending
    // on what we read in.
    map<int, string> groups;
    groups[1] = "YODA_HISTO1D";
    groups[2] = "YODA_HISTO2D";
    groups[3] = "YODA_PROFILE1D";
    groups[4] = "YODA_PROFILE2D";
    groups[5] = "YODA_SCATTER1D";
    groups[6] = "YODA_SCATTER2D";
    groups[7] = "YODA_SCATTER3D";
    groups[8] = "YODA_COUNTER";

    // Initialize the group parser
    pair <int, string> pis;  // To make boost's BOOST_FOREACH happy
    BOOST_FOREACH(pis, groups) {
      bgroup.add(pis.second, pis.first);
    }

    // The grammars for content (yoda) and context (group)
    yoda_grammar<string::iterator, ascii::space_type> yoda_parser;
    group_grammar<string::iterator> group_parser;

    // Now loop over all lines of the input file
    int context = 0;
    bool contextchange = false;
    string s;
    while (safe_getline(stream, s)) {
      // First check if we found a "# BEGIN ..." or "# END ..." line.
      // This marks a context change.
      int newcontext = 0;
      // if (qi::parse(s.begin(), s.end(), group_parser, newcontext)) { //< Only supported in Boost 1.47+
      string::iterator it1 = s.begin();
      if (qi::parse(it1, s.end(), group_parser, newcontext)) { //< End patch
        context = newcontext;
        if (context > 0) {
          // We are inside a group now, so we are looking for the corresponding END and ignore all BEGINs
          bgroup.clear();
          egroup.add(groups[context], -context);
        }
        if (context < 0) {
          // We are outside a group, so we are looking for any BEGIN and ignore all ENDs
          egroup.remove(groups[-context]);
          BOOST_FOREACH(pis, groups) bgroup.add(pis.second, pis.first);
          contextchange = true;
        }
      }


      /// @todo Finish and use this macro
      // #define _READERYODA_APPLY_ANNOTATIONS() {
      //     pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
      // BOOST_FOREACH (pss, _annotations)
      //   ao->setAnnotation(pss.first, pss.second);
      // aos.push_back(ao);
      // cleanup();
      // contextchange = false; }


      // Depending on the context, we either want to parse the line as data,
      // or to write out what we parsed so far (when leaving a group).
      switch (context) {
        case 1:  // we are inside YODA_HISTO1D
        case 2:  // we are inside YODA_HISTO2D
        case 3:  // we are inside YODA_PROFILE1D
        case 4:  // we are inside YODA_PROFILE2D
        case 5:  // we are inside YODA_SCATTER1D
        case 6:  // we are inside YODA_SCATTER2D
        case 7:  // we are inside YODA_SCATTER3D
        case 8:  // we are inside YODA_COUNTER
          {
            // if (! qi::phrase_parse(s.begin(), s.end(), yoda_parser, ascii::space) ) { //< Only supported in Boost 1.47+
            string::iterator it2 = s.begin();
            if (! qi::phrase_parse(it2, s.end(), yoda_parser, ascii::space) )
              cerr << "failed parsing this line:\n" << s << endl;
          }
          break;
        case -1: // we left YODA_HISTO1D
          if (contextchange) {
            YODA::AnalysisObject* ao = new YODA::Histo1D(_histo1d.bins, _histo1d.dbn_tot, _histo1d.dbn_uflow, _histo1d.dbn_oflow);
            pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
            BOOST_FOREACH (pss, _annotations)
              ao->setAnnotation(pss.first, pss.second);
            aos.push_back(ao);
            cleanup();
            contextchange = false;
          }
          break;
        case -2: // we left YODA_HISTO2D
          if (contextchange) {
            /// @todo For now just create 8 fake entries: needs to be greatly generalised for final form
            _histo2d.dbns_oflow.clear(); _histo2d.dbns_oflow.resize(8);
            YODA::AnalysisObject* ao = new YODA::Histo2D(_histo2d.bins, _histo2d.dbn_tot, _histo2d.dbns_oflow);
            pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
            BOOST_FOREACH (pss, _annotations)
              ao->setAnnotation(pss.first, pss.second);
            aos.push_back(ao);
            cleanup();
            contextchange = false;
          }
          break;
        case -3: // we left YODA_PROFILE1D
          if (contextchange) {
            YODA::AnalysisObject* ao = new YODA::Profile1D(_profile1d.bins, _profile1d.dbn_tot, _profile1d.dbn_uflow, _profile1d.dbn_oflow);
            pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
            BOOST_FOREACH (pss, _annotations)
              ao->setAnnotation(pss.first, pss.second);
            aos.push_back(ao);
            cleanup();
            contextchange = false;
          }
          break;
        case -4: // we left YODA_PROFILE2D
          if (contextchange) {
            /// @todo For now just create 8 fake entries: needs to be greatly generalised for final form
            _profile2d.dbns_oflow.clear(); _profile2d.dbns_oflow.resize(8);
            YODA::AnalysisObject* ao = new YODA::Profile2D(_profile2d.bins, _profile2d.dbn_tot, _profile2d.dbns_oflow);
            pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
            BOOST_FOREACH (pss, _annotations)
              ao->setAnnotation(pss.first, pss.second);
            aos.push_back(ao);
            cleanup();
            contextchange = false;
          }
          break;
        case -5: // we left YODA_SCATTER1D
          if (contextchange) {
            YODA::AnalysisObject* ao = new YODA::Scatter1D(_scatter1d.points);
            pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
            BOOST_FOREACH (pss, _annotations)
              ao->setAnnotation(pss.first, pss.second);
            aos.push_back(ao);
            cleanup();
            contextchange = false;
          }
          break;
        case -6: // we left YODA_SCATTER2D
          if (contextchange) {
            YODA::AnalysisObject* ao = new YODA::Scatter2D(_scatter2d.points);
            pair<string, string> pss;  // to make boost's BOOST_FOREACH happy
            BOOST_FOREACH (pss, _annotations)
              ao->setAnnotation(pss.first, pss.second);
            aos.push_back(ao);
            cleanup();
            contextchange = false;
          }
          break;
        // case -7: // we left YODA_SCATTER3D
          /// @todo We need to iprove the parser to read Scatter3D, since it has the same number of line items as a profile type
          cerr << "YODA WARNING: Scatter3D can't currently be read from .yoda format. "
               << "This should be fixed soon: please complain to the authors!" << endl;
      }
    }
  }


}
