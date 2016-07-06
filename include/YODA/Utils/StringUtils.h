// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_STRINGUTILS_H
#define YODA_STRINGUTILS_H

namespace YODA {
  namespace Utils {


    /// Replace
    inline std::string encodeForXML(const std::string& in) {
      std::string out = in;
      typedef std::pair<std::string, std::string> CharsToEntities;
      std::vector<CharsToEntities> cs2es;
      cs2es.push_back(std::make_pair("&", "&amp;"));
      cs2es.push_back(std::make_pair("<", "&lt;"));
      cs2es.push_back(std::make_pair(">", "&gt;"));

      for (std::vector<CharsToEntities>::const_iterator c2e = cs2es.begin(); c2e != cs2es.end(); ++c2e) {
        std::string::size_type pos = -1;
        while ( ( pos = out.find(c2e->first, pos + 1) ) != std::string::npos ) {
          out.replace(pos, 1, c2e->second);
        }
      }
      return out;
    }


  }
}

#endif
