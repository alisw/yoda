// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2016 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_READERAIDA_H
#define YODA_READERAIDA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Reader.h"

namespace YODA {


  /// Persistency reader for AIDA XML format.
  class ReaderAIDA : public Reader {
  public:

    /// Singleton creation function
    static Reader& create() {
      static ReaderAIDA _instance;
      return _instance;
    }

    void read(std::istream& stream, std::vector<AnalysisObject*>& aos) {
      _readDoc(stream, aos);
    }

  protected:

    void _readDoc(std::istream& stream, std::vector<AnalysisObject*>& aos);

  private:

    /// Private constructor, since it's a singleton.
    ReaderAIDA() { }

  };


}

#endif
