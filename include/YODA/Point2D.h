// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2017 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT2D_H
#define YODA_POINT2D_H

#include "YODA/Point.h"
#include "YODA/Exceptions.h"
#include "YODA/Utils/MathUtils.h"
#include <utility>

namespace YODA {


  /// A 2D data point to be contained in a Scatter2D
  class Point2D : public Point {
  public:

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

    /// Space dimension of the point
    size_t dim() { return 2; }


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
    void setXY(const std::pair<double,double>& xy) { setX(xy.first); setY(xy.second); }

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

    /// Set symmetric x error (alias)
    void setXErrs(double ex) {
      setXErr(ex);
    }

    /// Set asymmetric x error
    void setXErrs(double exminus, double explus) {
      setXErrMinus(exminus);
      setXErrPlus(explus);
    }

    /// Set asymmetric x error
    void setXErrs(const std::pair<double,double>& ex) {
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

    /// Set symmetric y error (alias)
    void setYErrs(double ey) {
      setYErr(ey);
    }

    /// Set asymmetric y error
    void setYErrs(double eyminus, double eyplus) {
      setYErrMinus(eyminus);
      setYErrPlus(eyplus);
    }

    /// Set asymmetric y error
    void setYErrs(const std::pair<double,double>& ey) {
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

    //@}


    /// @todo Support multiple errors


    /// @name Combined x/y value and error setters
    //@{

    /// Set x value and symmetric error
    void setX(double x, double ex) {
      setX(x);
      setXErrs(ex);
    }

    /// Set x value and asymmetric error
    void setX(double x, double exminus, double explus) {
      setX(x);
      setXErrs(exminus, explus);
    }

    /// Set x value and asymmetric error
    void setX(double x, std::pair<double,double>& ex) {
      setX(x);
      setXErrs(ex);
    }


    /// Set y value and symmetric error
    void setY(double y, double ey) {
      setY(y);
      setYErrs(ey);
    }

    /// Set y value and asymmetric error
    void setY(double y, double eyminus, double eyplus) {
      setY(y);
      setYErrs(eyminus, eyplus);
    }

    /// Set y value and asymmetric error
    void setY(double y, std::pair<double,double>& ey) {
      setY(y);
      setYErrs(ey);
    }

    //@}


    // @name Manipulations
    //@{

    /// Scaling of x axis
    void scaleX(double scalex) {
      setX(x()*scalex);
      setXErrs(xErrMinus()*scalex, xErrPlus()*scalex);
    }

    /// Scaling of y axis
    void scaleY(double scaley) {
      setY(y()*scaley);
      setYErrs(yErrMinus()*scaley, yErrPlus()*scaley);
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


    /// @name Integer axis accessor equivalents
    //@{

    /// Get the point value for direction @a i
    double val(size_t i) const {
      switch (i) {
      case 1: return x();
      case 2: return y();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set the point value for direction @a i
    void setVal(size_t i, double val) {
      switch (i) {
      case 1: setX(val); break;
      case 2: setY(val); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    /// Get error values for direction @a i
    const std::pair<double,double>& errs(size_t i) const {
      switch (i) {
      case 1: return xErrs();
      case 2: return yErrs();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Get negative error value for direction @a i
    double errMinus(size_t i) const {
      switch (i) {
      case 1: return xErrMinus();
      case 2: return yErrMinus();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Get positive error value for direction @a i
    double errPlus(size_t i) const {
      switch (i) {
      case 1: return xErrPlus();
      case 2: return yErrPlus();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Get average error value for direction @a i
    double errAvg(size_t i) const {
      switch (i) {
      case 1: return xErrAvg();
      case 2: return yErrAvg();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    /// Set negative error for direction @a i
    void setErrMinus(size_t i, double eminus) {
      switch (i) {
      case 1: setXErrMinus(eminus); break;
      case 2: setYErrMinus(eminus); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set positive error for direction @a i
    void setErrPlus(size_t i, double eplus) {
      switch (i) {
      case 1: setXErrPlus(eplus); break;
      case 2: setYErrPlus(eplus); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    /// Set symmetric error for direction @a i
    void setErr(size_t i, double e) {
      switch (i) {
      case 1: setXErrs(e); break;
      case 2: setYErrs(e); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set asymmetric error for direction @a i
    void setErrs(size_t i, double eminus, double eplus) {
      switch (i) {
      case 1: setXErrs(eminus, eplus); break;
      case 2: setYErrs(eminus, eplus); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set asymmetric error for direction @a i
    void setErrs(size_t i, std::pair<double,double>& e) {
      switch (i) {
      case 1: setXErrs(e); break;
      case 2: setYErrs(e); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    /// Set value and symmetric error for direction @a i
    void set(size_t i, double val, double e) {
      switch (i) {
      case 1: setX(val, e); break;
      case 2: setY(val, e); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set value and asymmetric error for direction @a i
    void set(size_t i, double val, double eminus, double eplus) {
      switch (i) {
      case 1: setX(val, eminus, eplus); break;
      case 2: setY(val, eminus, eplus); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set value and asymmetric error for direction @a i
    void set(size_t i, double val, std::pair<double,double>& e) {
      switch (i) {
      case 1: setX(val, e); break;
      case 2: setY(val, e); break;
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
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
  /// @todo Need to add y comparisons, too
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
