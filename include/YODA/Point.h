// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2017 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT_H
#define YODA_POINT_H

#include "YODA/AnalysisObject.h"

namespace YODA {


  /// Base class for all Point*Ds, providing generic access to their numerical properties
  class Point {
  public:

    typedef std::pair<double,double> ValuePair;


    /// Virtual destructor for inheritance
    virtual ~Point() {};


    /// Space dimension of the point
    virtual size_t dim() = 0;


    /// Get the point value for direction @a i
    virtual double val(size_t i) const = 0;
    /// Set the point value for direction @a i
    virtual void setVal(size_t i, double val) = 0;

    /// Get error values for direction @a i
    virtual const std::pair<double,double>& errs(size_t i) const = 0;
    /// Set symmetric error for direction @a i
    virtual void setErr(size_t i, double e) = 0;
    /// Set symmetric error for direction @a i (alias)
    virtual void setErrs(size_t i, double e) { return setErr(i,e); }
    /// Set asymmetric error for direction @a i
    virtual void setErrs(size_t i, double eminus, double eplus) = 0;
    /// Set asymmetric error for direction @a i
    virtual void setErrs(size_t i, std::pair<double,double>& e) = 0;

    /// Get negative error value for direction @a i
    virtual double errMinus(size_t i) const = 0;
    /// Set negative error for direction @a i
    virtual void setErrMinus(size_t i, double eminus) = 0;

    /// Get positive error value for direction @a i
    virtual double errPlus(size_t i) const = 0;
    /// Set positive error for direction @a i
    virtual void setErrPlus(size_t i, double eplus) = 0;

    /// Get average error value for direction @a i
    virtual double errAvg(size_t i) const = 0;

    // /// Get value minus negative error for direction @a i
    // double min(size_t i) const = 0;
    // /// Get value plus positive error for direction @a i
    // double max(size_t i) const = 0;

    //@}

    /// @todo Support multiple errors

    /// @name Combined value and error setters
    //@{

    /// Set value and symmetric error for direction @a i
    virtual void set(size_t i, double val, double e) = 0;
    /// Set value and asymmetric error for direction @a i
    virtual void set(size_t i, double val, double eminus, double eplus) = 0;
    /// Set value and asymmetric error for direction @a i
    virtual void set(size_t i, double val, std::pair<double,double>& e) = 0;

    //@}


    // @name Manipulations
    //@{

    // /// Scaling of direction @a i
    // void scale(size_t i, double scale) = 0;

    /// @todo void transform(size_t i, FN f) = 0;

    //@}

  };


}

#endif
