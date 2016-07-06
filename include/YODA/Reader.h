// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Reader_h
#define YODA_Reader_h

#include "YODA/AnalysisObject.h"
#include <string>
#include <fstream>
#include <vector>

namespace YODA {


  /// Pure virtual base class for various output writers.
  class Reader {
  public:

    /// Virtual destructor
    virtual ~Reader() {}


    /// @name Reading multiple analysis objects,
    //@{

    /// @brief Read in a collection of objects @a objs from output stream @a stream.
    ///
    /// This version fills (actually, appends to) a supplied vector, avoiding copying,
    /// and is hence CPU efficient.
    ///
    /// @todo Use SFINAE magic to allow ~arbitrary collection<AnalysisObject*> (with push_back()?) to be passed
    virtual void read(std::istream& stream, std::vector<AnalysisObject*>& aos) = 0;

    /// @brief Read in a collection of objects from output stream @a stream.
    ///
    /// This version returns a vector by value, involving copying, and is hence less
    /// CPU efficient than the alternative version where a vector is filled by reference.
    std::vector<AnalysisObject*> read(std::istream& stream) {
      std::vector<AnalysisObject*> rtn;
      read(stream, rtn);
      return rtn;
    }

    /// @brief Read in a collection of objects @a objs from file @a filename.
    ///
    /// This version fills (actually, appends to) a supplied vector, avoiding copying,
    /// and is hence CPU efficient.
    ///
    /// @todo Use SFINAE magic to allow ~arbitrary collection<AnalysisObject*> (with push_back()?) to be passed
    void read(const std::string& filename, std::vector<AnalysisObject*>& aos) {
      std::ifstream instream;
      instream.open(filename.c_str());
      read(instream, aos);
      instream.close();
    }

    /// @brief Read in a collection of objects from output stream @a stream.
    ///
    /// This version returns a vector by value, involving copying, and is hence less
    /// CPU efficient than the alternative version where a vector is filled by reference.
    std::vector<AnalysisObject*> read(const std::string& filename) {
      std::vector<AnalysisObject*> rtn;
      read(filename, rtn);
      return rtn;
    }

    //@}


  };


  /// Factory function to make a writer object by format name or a filename
  Reader& mkReader(const std::string& format_name);


}

#endif
