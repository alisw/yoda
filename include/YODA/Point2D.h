// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT2D_H
#define YODA_POINT2D_H

#include "YODA/Exceptions.h"
#include "YODA/Utils/MathUtils.h"
#include <utility>

namespace YODA {


  /// A 2D data point to be contained in a Scatter2D
  class Point2D {
  public:

    typedef std::pair<double,double> ValuePair;


    /// @name Constructors
    //@{

    // Default constructor
    Point2D() {  }


    /// Constructor from values with optional symmetric errors
    Point2D(double x, double y, double ex=0.0, double ey=0.0)
      : _x(x), _y(y)
    {
      _ex = std::make_pair(ex, ex);
      _ey = std::make_pair(ey, ey);
    }


    /// Constructor from values with explicit asymmetric errors
    Point2D(double x, double y,
            double exminus,
            double explus,
            double eyminus,
            double eyplus)
      : _x(x), _y(y)
    {
      _ex = std::make_pair(exminus, explus);
      _ey = std::make_pair(eyminus, eyplus);
    }


    // /// Constructor from values with symmetric errors on x and asymmetric errors on y
    // Point2D(double x, double y, double ex, const std::pair<double,double>& ey)
    //   : _x(x), _y(y), _ey(ey)
    // {
    //   _ex = std::make_pair(ex, ex);
    // }


    // /// Constructor from values with asymmetric errors on x and symmetric errors on y
    // Point2D(double x, double y, const std::pair<double,double>& ex, double ey)
    //   : _x(x), _y(y), _ex(ex)
    // {
    //   _ey = std::make_pair(ey, ey);
    // }


    /// Constructor from values with asymmetric errors on both x and y
    Point2D(double x, double y, const std::pair<double,double>& ex, const std::pair<double,double>& ey)
      : _x(x), _y(y), _ex(ex), _ey(ey)
    {  }


    /// Copy constructor
    Point2D(const Point2D& p)
      : _x(p._x), _y(p._y),
        _ex(p._ex), _ey(p._ey)
    {  }


    /// Copy assignment
    Point2D& operator = (const Point2D& p) {
      _x = p._x;
      _y = p._y;
      _ex = p._ex;
      _ey = p._ey;
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

    /// Get y value
    double y() const { return _y; }

    /// Set y value
    void setY(double y) { _y = y; }

    /// @todo Uniform "coords" accessor across all Scatters: returning fixed-size tuple?

    /// Get x,y value pair
    std::pair<double,double> xy() const { return std::make_pair(_x, _y); }

    /// Set x and y values
    void setXY(double x, double y) { setX(x); setY(y); }

    /// Set x and y values
    void setXY(std::pair<double,double> xy) { setX(xy.first); setY(xy.second); }

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


    /// @name y error accessors
    //@{

    /// Get y-error values
    const std::pair<double,double>& yErrs() const {
      return _ey;
    }

    /// Get negative y-error value
    double yErrMinus() const {
      return _ey.first;
    }

    /// Get positive y-error value
    double yErrPlus() const {
      return _ey.second;
    }

    /// Get average y-error value
    double yErrAvg() const {
      return (_ey.first + _ey.second)/2.0;
    }

    /// Set negative y error
    void setYErrMinus(double eyminus) {
      _ey.first = eyminus;
    }

    /// Set positive y error
    void setYErrPlus(double eyplus) {
      _ey.second = eyplus;
    }

    /// Set symmetric y error
    void setYErr(double ey) {
      setYErrMinus(ey);
      setYErrPlus(ey);
    }

    /// Set asymmetric y error
    void setYErr(double eyminus, double eyplus) {
      setYErrMinus(eyminus);
      setYErrPlus(eyplus);
    }

    /// Set asymmetric y error
    void setYErr(std::pair<double,double> ey) {
      _ey = ey;
    }

    /// Get value minus negative y-error
    /// @todo Remove (or extend) when multiple errors are supported
    double yMin() const {
      return _y - _ey.first;
    }

    /// Get value plus positive y-error
    /// @todo Remove (or extend) when multiple errors are supported
    double yMax() const {
      return _y + _ey.second;
    }

    // /// Set negative y-error via the yMin position
    // /// @todo Remove when multiple errors are supported
    // void setYMin(double ymin) {
    //   if (ymin > y()) throw UserError("Attempted to set an ymin > y");
    //   setYErrMinus(y() - ymin);
    // }

    // /// Set positive y-error via the yMax position
    // /// @todo Remove when multiple errors are supported
    // void setYMax(double ymax) {
    //   if (ymax < y()) throw UserError("Attempted to set an ymax < y");
    //   setYErrPlus(ymax - y());
    // }

    //@}


    /// @todo Support multiple errors


    /// @name Combined x/y value and error setters
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


    /// Set y value and symmetric error
    void setY(double y, double ey) {
      setY(y);
      setYErr(ey);
    }

    /// Set y value and asymmetric error
    void setY(double y, double eyminus, double eyplus) {
      setY(y);
      setYErr(eyminus, eyplus);
    }

    /// Set y value and asymmetric error
    void setY(double y, std::pair<double,double> ey) {
      setY(y);
      setYErr(ey);
    }

    //@}


    // @name Manipulations
    //@{

    /// Scaling of x axis
    void scaleX(double scalex) {
      setX(x()*scalex);
      setXErr(xErrMinus()*scalex, xErrPlus()*scalex);
    }

    /// Scaling of y axis
    void scaleY(double scaley) {
      setY(y()*scaley);
      setYErr(yErrMinus()*scaley, yErrPlus()*scaley);
    }

    /// Scaling of both axes
    void scaleXY(double scalex, double scaley) {
      scaleX(scalex);
      scaleY(scaley);
    }

    /// Scaling of both axes
    /// @deprecated Use scaleXY
    void scale(double scalex, double scaley) {
      scaleXY(scalex, scaley);
    }

    //@}


  protected:

    /// @name Value and error variables
    //@{

    double _x;
    double _y;
    std::pair<double,double> _ex;
    std::pair<double,double> _ey;

    //@}

  };



  /// @name Comparison operators
  //@{

  /// Equality test of x characteristics only
  inline bool operator==(const YODA::Point2D& a, const YODA::Point2D& b) {
    const bool same_val = YODA::fuzzyEquals(a.x(), b.x());
    const bool same_eminus = YODA::fuzzyEquals(a.xErrMinus(), b.xErrMinus());
    const bool same_eplus = YODA::fuzzyEquals(a.xErrPlus(), b.xErrPlus());
    return same_val && same_eminus && same_eplus;
  }

  /// Equality test of x characteristics only
  inline bool operator != (const YODA::Point2D& a, const YODA::Point2D& b) {
    return !(a == b);
  }

  /// Less-than operator used to sort bins by x-ordering
  inline bool operator < (const YODA::Point2D& a, const YODA::Point2D& b) {
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
  inline bool operator <= (const YODA::Point2D& a, const YODA::Point2D& b) {
    if (a == b) return true;
    return a < b;
  }

  /// Greater-than operator used to sort bins by x-ordering
  inline bool operator > (const YODA::Point2D& a, const YODA::Point2D& b) {
    return !(a <= b);
  }

  /// Greater-than-or-equals operator used to sort bins by x-ordering
  inline bool operator >= (const YODA::Point2D& a, const YODA::Point2D& b) {
    return !(a < b);
  }

  //@}


}

#endif
