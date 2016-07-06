// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2016 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Writer.h"
#include "YODA/WriterYODA.h"
#include "YODA/WriterAIDA.h"
#include "YODA/WriterFLAT.h"
#include <iostream>
#include <typeinfo>
#include <sstream>

using namespace std;

namespace YODA {


  Writer& mkWriter(const std::string& name) {
    const size_t lastdot = name.find_last_of(".");
    const string fmt = Utils::toLower((lastdot == std::string::npos) ? name : name.substr(lastdot+1));
    // cout << "File extension: " << fmt << endl;
    if (fmt == "yoda") return WriterYODA::create();
    if (fmt == "aida") return WriterAIDA::create();
    if (fmt == "dat" || fmt == "flat") return WriterFLAT::create();
    throw UserError("Format cannot be identified from string '" + name + "'");
  }


  void Writer::write(std::ostream& stream, const AnalysisObject& ao) {
    writeHeader(stream);
    writeBody(stream, ao);
    writeFooter(stream);
  }


  void Writer::write(const std::string& filename, const AnalysisObject& ao) {
    ofstream outstream;
    outstream.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try {
      outstream.open(filename.c_str());
      write(outstream, ao);
      outstream.close();
    } catch(std::ifstream::failure e) {
      throw WriteError("writing to filename " + filename + " failed: " + e.what());
    }
  }

  void Writer::writeBody(std::ostream& stream, const AnalysisObject* ao) {
    if(!ao) throw WriteError("attempting to write a null AnalysisObject");
    writeBody(stream,*ao);
  }

  void Writer::writeBody(std::ostream& stream, const AnalysisObject& ao) {
    const string aotype = ao.type();
    if (aotype == "Counter") {
      writeCounter(stream, dynamic_cast<const Counter&>(ao));
    } else if (aotype == "Histo1D") {
      writeHisto1D(stream, dynamic_cast<const Histo1D&>(ao));
    } else if (aotype == "Histo2D") {
      writeHisto2D(stream, dynamic_cast<const Histo2D&>(ao));
    } else if (aotype == "Profile1D") {
      writeProfile1D(stream, dynamic_cast<const Profile1D&>(ao));
    } else if (aotype == "Profile2D") {
      writeProfile2D(stream, dynamic_cast<const Profile2D&>(ao));
    } else if (aotype == "Scatter1D") {
      writeScatter1D(stream, dynamic_cast<const Scatter1D&>(ao));
    } else if (aotype == "Scatter2D") {
      writeScatter2D(stream, dynamic_cast<const Scatter2D&>(ao));
    } else if (aotype == "Scatter3D") {
      writeScatter3D(stream, dynamic_cast<const Scatter3D&>(ao));
    } else if (aotype[0] == '_') {
      // skip writing AO types with underscore prefixes (needed e.g. for Rivet wrappers)
      // maybe write a comment line in the output
    } else {
      ostringstream oss;
      oss << "Unrecognised analysis object type " << aotype << " in Writer::write";
      throw Exception(oss.str());
    }
  }


}
