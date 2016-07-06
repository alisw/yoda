// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2016 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_READER_H
#define YODA_READER_H

#include "YODA/AnalysisObject.h"
#include "YODA/Utils/Traits.h"
#include <string>
#include <fstream>
#include <vector>
#include <type_traits>
#include <iostream>

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
    /// This version fills (actually, appends to) a variable supplied container
    /// Note: SFINAE is used to check for a void push_back(const AnalysisObject*) method
    template<typename CONT>
    typename std::enable_if<YODA::Pushable<CONT,AnalysisObject*>::value>::type
    read(std::istream& stream, CONT& aos) {
      //if CONT==std::vector<AnalysisObject*>, the compiler should select
      //the virtual method below, since it prefers non-templated methods in the lookup
      //otherwise we would enter a endless recursion. Check in case of problems.
      std::vector<AnalysisObject*> v_aos;
      read(stream,v_aos);
      std::vector<AnalysisObject*>::const_iterator it = v_aos.begin();
      for(it = v_aos.begin(); it!=v_aos.end(); ++it){
        aos.push_back(*it);
      }
    }

    /// @brief Read in a collection of objects @a objs from output stream @a stream.
    ///
    /// This version fills (actually, appends to) a supplied vector, avoiding copying,
    /// and is hence CPU efficient.
    ///
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
