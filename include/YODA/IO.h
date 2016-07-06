// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_IO_h
#define YODA_IO_h

#include "YODA/Writer.h"
#include "YODA/Reader.h"

namespace YODA {


  /// @name Writer functions with automatic format detection
  //@{

  /// Write out object @a ao to file @a filename
  void write(const std::string& filename, const AnalysisObject& ao) {
    Writer& w = mkWriter(filename);
    w.write(filename, ao);
  }

  /// Write out a collection of objects @a objs to file @a filename.
  template <typename RANGE>
  void write(const std::string& filename, const RANGE& aos) {
    Writer& w = mkWriter(filename);
    w.write(filename, aos);
  }

  /// Write out the objects specified by start iterator @a begin and end
  /// iterator @a end to file @a filename.
  template <typename AOITER>
  void write(const std::string& filename, const AOITER& begin, const AOITER& end) {
    Writer& w = mkWriter(filename);
    w.write(filename, begin, end);
  }

  //@}


  /// @name Reader functions with automatic format detection
  //@{

  /// @brief Read in a collection of objects @a objs from file @a filename.
  ///
  /// This version fills (actually, appends to) a supplied vector, avoiding
  /// copying, and is hence CPU efficient. The appropriate format reader will
  /// be determined from the filename.
  ///
  /// @todo Use SFINAE magic to allow ~arbitrary collection<AnalysisObject*> (with push_back()?) to be passed
  void read(const std::string& filename, std::vector<AnalysisObject*>& aos) {
    Reader& r = mkReader(filename);
    r.read(filename, aos);
  }

  /// @brief Read in a collection of objects from output stream @a stream.
  ///
  /// This version returns a vector by value, involving copying, and is hence
  /// less CPU efficient than the alternative version where a vector is filled
  /// by reference. The appropriate format reader will be determined from the
  /// filename.
  std::vector<AnalysisObject*> read(const std::string& filename) {
    std::vector<AnalysisObject*> rtn;
    read(filename, rtn);
    return rtn;
  }

  //@}


}

#endif
