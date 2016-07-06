// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT1D_H
#define YODA_POINT1D_H

#include "YODA/Exceptions.h"
#include "YODA/Utils/MathUtils.h"
#include <utility>

namespace YODA {


  /// A 1D data point to be contained in a Scatter1D
  class Point1D {
  public:

    typedef std::pair<double,double> ValuePair;


    /// @name Constructors
    //@{

    // Default constructor
    Point1D() {  }


    /// Constructor from values with optional symmetric errors
    Point1D(double x, double ex=0.0)
      : _x(x)
    {
      _ex = std::make_pair(ex, ex);
    }


    /// Constructor from values with explicit asymmetric errors
    Point1D(double x, double exminus, double explus)
      : _x(x)
    {
      _ex = std::make_pair(exminus, explus);
    }


    /// Constructor from values with asymmetric errors
    Point1D(double x, const std::pair<double,double>& ex)
      : _x(x), _ex(ex)
    {  }


    /// Copy constructor
    Point1D(const Point1D& p)
      : _x(p._x), _ex(p._ex)
    {  }


    /// Copy assignment
    Point1D& operator = (const Point1D& p) {
      _x = p._x;
      _ex = p._ex;
      return *this;
    }

    //@}


  public:

    /// @name Value accessors
    //@{

    /// Get x value
    double x() const { return _x; }

    /// Set x value
    void setX(double x) { _x = x; }

    /// @todo Uniform "coords" accessor across all Scatters: returning fixed-size tuple?

    //@}


    /// @name x error accessors
    //@{

    /// Get x-error values
    const std::pair<double,double>& xErrs() const {
      return _ex;
    }

    /// Get negative x-error value
    double xErrMinus() const {
      return _ex.first;
    }

    /// Get positive x-error value
    double xErrPlus() const {
      return _ex.second;
    }

    /// Get average x-error value
    double xErrAvg() const {
      return (_ex.first + _ex.second)/2.0;
    }

    /// Set negative x error
    void setXErrMinus(double exminus) {
      _ex.first = exminus;
    }

    /// Set positive x error
    void setXErrPlus(double explus) {
      _ex.second = explus;
    }

    /// Set symmetric x error
    void setXErr(double ex) {
      setXErrMinus(ex);
      setXErrPlus(ex);
    }

    /// Set asymmetric x error
    void setXErr(double exminus, double explus) {
      setXErrMinus(exminus);
      setXErrPlus(explus);
    }

    /// Set asymmetric x error
    void setXErr(std::pair<double,double> ex) {
      _ex = ex;
    }

    /// Get value minus negative x-error
    /// @todo Remove (or extend) when multiple errors are supported
    double xMin() const {
      return _x - _ex.first;
    }

    /// Get value plus positive x-error
    /// @todo Remove (or extend) when multiple errors are supported
    double xMax() const {
      return _x + _ex.second;
    }

    // /// Set negative x-error via the xMin position
    // /// @todo Remove when multiple errors are supported
    // void setXMin(double xmin) {
    //   if (xmin > x()) throw UserError("Attempted to set an xmin > x");
    //   setXErrMinus(x() - xmin);
    // }

    // /// Set positive x-error via the xMax position
    // /// @todo Remove when multiple errors are supported
    // void setXMax(double xmax) {
    //   if (xmax < x()) throw UserError("Attempted to set an xmax < x");
    //   setXErrPlus(xmax - x());
    // }

    //@}


    /// @todo Support multiple errors


    /// @name Combined x value and error setters
    //@{

    /// Set x value and symmetric error
    void setX(double x, double ex) {
      setX(x);
      setXErr(ex);
    }

    /// Set x value and asymmetric error
    void setX(double x, double exminus, double explus) {
      setX(x);
      setXErr(exminus, explus);
    }

    /// Set x value and asymmetric error
    void setX(double x, std::pair<double,double> ex) {
      setX(x);
      setXErr(ex);
    }

    //@}


    // @name Manipulations
    //@{

    /// Scaling of x axis
    void scaleX(double scalex) {
      setX(x()*scalex);
      setXErr(xErrMinus()*scalex, xErrPlus()*scalex);
    }

    //@}


  protected:

    /// @name Value and error variables
    //@{

    double _x;
    std::pair<double,double> _ex;

    //@}

  };



  /// @name Comparison operators
  //@{

  /// Equality test of x characteristics only
  /// @todo AAARGH! Need to also test y. Maybe need named cmp operators for separate x/y and with/without metadata
  inline bool operator==(const YODA::Point1D& a, const YODA::Point1D& b) {
    const bool same_val = YODA::fuzzyEquals(a.x(), b.x());
    const bool same_eminus = YODA::fuzzyEquals(a.xErrMinus(), b.xErrMinus());
    const bool same_eplus = YODA::fuzzyEquals(a.xErrPlus(), b.xErrPlus());
    return same_val && same_eminus && same_eplus;
  }

  /// Equality test of x characteristics only
  /// @todo AAARGH! Need to also test y. Maybe need named cmp operators for separate x/y and with/without metadata
  inline bool operator != (const YODA::Point1D& a, const YODA::Point1D& b) {
    return !(a == b);
  }

  /// Less-than operator used to sort bins by x-ordering
  inline bool operator < (const YODA::Point1D& a, const YODA::Point1D& b) {
    if (!YODA::fuzzyEquals(a.x(), b.x())) {
      return a.x() < b.x();
    }
    if (!YODA::fuzzyEquals(a.xErrMinus(), b.xErrMinus())) {
      return a.xErrMinus() < b.xErrMinus();
    }
    if (!YODA::fuzzyEquals(a.xErrPlus(), b.xErrPlus())) {
      return a.xErrPlus() < b.xErrPlus();
    }
    return false;
  }

  /// Less-than-or-equals operator used to sort bins by x-ordering
  inline bool operator <= (const YODA::Point1D& a, const YODA::Point1D& b) {
    if (a == b) return true;
    return a < b;
  }

  /// Greater-than operator used to sort bins by x-ordering
  inline bool operator > (const YODA::Point1D& a, const YODA::Point1D& b) {
    return !(a <= b);
  }

  /// Greater-than-or-equals operator used to sort bins by x-ordering
  inline bool operator >= (const YODA::Point1D& a, const YODA::Point1D& b) {
    return !(a < b);
  }

  //@}


}

#endif
