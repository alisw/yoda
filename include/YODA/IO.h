// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2018 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_IO_h
#define YODA_IO_h

#include "YODA/Writer.h"
#include "YODA/Reader.h"

namespace YODA {


  /// @name Writer functions to files (with automatic format detection)
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


  /// @name Writer functions to streams (with explicit format specification)
  //@{

  /// Write out object @a ao to stream @a os with format @a fmt
  void write(std::ostream& os, const AnalysisObject& ao, const std::string& fmt) {
    Writer& w = mkWriter(fmt);
    w.write(os, ao);
  }

  /// Write out a collection of objects @a objs to file @a filename.
  template <typename RANGE>
  void write(std::ostream& os, const RANGE& aos, const std::string& fmt) {
    Writer& w = mkWriter(fmt);
    w.write(os, aos);
  }

  /// Write out the objects specified by start iterator @a begin and end
  /// iterator @a end to file @a filename.
  template <typename AOITER>
  void write(std::ostream& os, const AOITER& begin, const AOITER& end, const std::string& fmt) {
    Writer& w = mkWriter(fmt);
    w.write(os, begin, end);
  }

  //@}


  /// @name Reader functions from files (with automatic format detection)
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

  /// @brief Read in a collection of objects from file @a filename.
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


  /// @name Reader functions from streams (with explicit format specification)
  //@{

  /// @brief Read in a collection of objects @a objs from stream @a is, expecting format @a fmt.
  ///
  /// This version fills (actually, appends to) a supplied vector, avoiding
  /// copying, and is hence CPU efficient.
  ///
  /// @todo Use SFINAE magic to allow ~arbitrary collection<AnalysisObject*> (with push_back()?) to be passed
  void read(std::istream& is, std::vector<AnalysisObject*>& aos, const std::string& fmt) {
    Reader& r = mkReader(fmt);
    r.read(is, aos);
  }

  /// @brief Read in a collection of objects from stream @a is, expecting format @a fmt.
  ///
  /// This version returns a vector by value, involving copying, and is hence
  /// less CPU efficient than the alternative version where a vector is filled
  /// by reference.
  std::vector<AnalysisObject*> read(std::istream& is, const std::string& fmt) {
    std::vector<AnalysisObject*> rtn;
    read(is, rtn, fmt);
    return rtn;
  }

  //@}


}

#endif
