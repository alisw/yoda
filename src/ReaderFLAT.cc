// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/ReaderFLAT.h"
#include "YODA/Utils/StringUtils.h"
#include "YODA/Exceptions.h"

#include "YODA/Counter.h"
#include "YODA/Scatter1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Scatter3D.h"

#include <iostream>
using namespace std;

namespace YODA {

  qi::symbols<char, int> ReaderFLAT::bgroup;
  qi::symbols<char, int> ReaderFLAT::egroup;
  //ReaderFLAT::counter ReaderFLAT::_counter;
  ReaderFLAT::scatter1d ReaderFLAT::_scatter1d;
  ReaderFLAT::scatter2d ReaderFLAT::_scatter2d;
  ReaderFLAT::scatter3d ReaderFLAT::_scatter3d;
  map<string, string> ReaderFLAT::_annotations;


  namespace {

    /// @todo Move to Utils?
    // Portable version of getline taken from
    // http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
    istream& safe_getline(istream& is, string& t) {
      t.clear();

      // The characters in the stream are read one-by-one using a streambuf.
      // That is faster than reading them one-by-one using the istream.
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


  void ReaderFLAT::_readDoc(istream& stream, vector<AnalysisObject*>& aos) {

    // These are the context groups we know. We need
    // that map to dynamically change the parser depending
    // on what we read in.
    map<int, string> groups;
    groups[1] = "HISTOGRAM";
    groups[2] = "HISTO1D";
    groups[3] = "HISTO2D";
    groups[4] = "COUNTER";

    // Initialize the group parser
    pair <int, string> pis;  // To make boost's BOOST_FOREACH happy
    BOOST_FOREACH(pis, groups) {
      bgroup.add(pis.second, pis.first);
    }

    // The grammars for content (data) and context (group)
    data_grammar<string::iterator, ascii::space_type> data_parser;
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
          //cout << "New context = " << context << endl;
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

      // Depending on the context, we either want to parse the line as data,
      // or to write out what we parsed so far (when leaving a group).
      switch (context) {
        case 1:  // we are inside HISTOGRAM
        case 2:  // we are inside HISTO1D
        case 3:  // we are inside HISTO2D
        case 4:  // we are inside COUNTER
          // if (! qi::phrase_parse(s.begin(), s.end(), data_parser, ascii::space) ) { //< Only supported in Boost 1.47+
          { //< Why the explicit scoping? Added by supplied patch from Andrii Verbytskyi
            string::iterator it2 = s.begin();
            if (! qi::phrase_parse(it2, s.end(), data_parser, ascii::space) ) { //< End patch
              cerr << "failed parsing this line:\n" << s << endl;
            }
          } //< End patch scoping
          break;
        case -1:  // we left HISTOGRAM
        case -2:  // we left HISTO1D
        case -3:  // we left HISTO2D
          if (contextchange) {
            YODA::AnalysisObject* ao = NULL;
            if (!_scatter1d.points.empty()) {
              // cout << "S1D" << endl;
              ao = new YODA::Scatter1D(_scatter1d.points);
            } else if (!_scatter2d.points.empty()) {
              // cout << "S2D" << endl;
              ao = new YODA::Scatter2D(_scatter2d.points);
            } else if (!_scatter3d.points.empty()) {
              // cout << "S3D" << endl;
              ao = new YODA::Scatter3D(_scatter3d.points);
            } else {
              throw YODA::ReadError("No data points found in flat histogram reading!");
            }
            pair<string, string> pss; // to make boost's BOOST_FOREACH happy
            BOOST_FOREACH (pss, _annotations) ao->setAnnotation(pss.first, pss.second);
            aos.push_back(ao);
            cleanup();
            contextchange = false;
          }
          break;
        case -4:  // we left COUNTER
          /// @todo Complete when context-sensitive parsing available, to avoid clash with symm H0D
          // if (contextchange) {
          //   YODA::AnalysisObject* ao = NULL;
          //   ao = new YODA::Counter(_counter);
          //   pair<string, string> pss; // to make boost's BOOST_FOREACH happy
          //   BOOST_FOREACH (pss, _annotations) ao->setAnnotation(pss.first, pss.second);
          //   aos.push_back(ao);
          //   cleanup();
          //   contextchange = false;
          // }
          break;
      }
    }
  }


}
