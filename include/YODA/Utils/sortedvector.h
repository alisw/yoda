// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SORTEDVECTOR_H
#define YODA_SORTEDVECTOR_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace YODA {
  namespace Utils {


    /// Specialisation of std::vector to allow indexed access to ordered elements
    ///
    /// @todo Need to template on the value-comparison definition?
    template <typename T>
    class sortedvector : public std::vector<T> {
    public:

      /// Default constructor
      sortedvector() {}

      /// Conversion from std::vector
      sortedvector(const std::vector<T> & vec)
        : std::vector<T>(vec) {
        std::sort(this->begin(), this->end());
      }

      /// Insertion operator (push_back should not be used!)
      void insert(const T& val) {
        std::vector<T>::push_back(val);
        std::sort(this->begin(), this->end());
      }


    private:

      /// Hiding push_back from the base class
      void push_back();

    };


  }
}

#endif
