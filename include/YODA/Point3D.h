// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2016 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_POINT3D_H
#define YODA_POINT3D_H

#include "YODA/Point.h"
#include "YODA/Exceptions.h"
#include "YODA/Utils/MathUtils.h"
#include <utility>

namespace YODA {


  /// A 3D data point to be contained in a Scatter3D
  class Point3D : public Point {
  public:

    /// @name Constructors
    //@{

    // Default constructor
    Point3D() {  }


    /// Constructor from values with optional symmetric errors
    Point3D(double x, double y, double z, double ex=0.0, double ey=0.0, double ez=0.0)
      : _x(x), _y(y), _z(z)
    {
      _ex = std::make_pair(ex, ex);
      _ey = std::make_pair(ey, ey);
      _ez = std::make_pair(ez, ez);
    }


    /// Constructor from values with explicit asymmetric errors
    Point3D(double x, double y, double z,
            double exminus,
            double explus,
            double eyminus,
            double eyplus,
            double ezminus,
            double ezplus)
      : _x(x), _y(y), _z(z)
    {
      _ex = std::make_pair(exminus, explus);
      _ey = std::make_pair(eyminus, eyplus);
      _ez = std::make_pair(ezminus, ezplus);
    }

    /// Constructor from asymmetric errors given as vectors
    Point3D(double x, double y, double z,
            const std::pair<double,double>& ex,
            const std::pair<double,double>& ey,
            const std::pair<double,double>& ez)
      : _x(x), _y(y), _z(z),
        _ex(ex), _ey(ey), _ez(ez)
    {  }


    /// Copy constructor
    Point3D(const Point3D& p)
      : _x(p._x), _y(p._y), _z(p._z),
        _ex(p._ex), _ey(p._ey), _ez(p._ez)
    {  }


    /// Copy assignment
    Point3D& operator = (const Point3D& p) {
      _x = p._x;
      _y = p._y;
      _z = p._z;
      _ex = p._ex;
      _ey = p._ey;
      _ez = p._ez;
      return *this;
    }


    //@}


  public:

    /// Space dimension of the point
    size_t dim() { return 3; }


    /// @name Value and error accessors
    //@{

    /// Get x value
    double x() const { return _x; }

    /// Set x value
    void setX(double x) { _x = x; }

    /// Get y value
    double y() const { return _y; }

    /// Set y value
    void setY(double y) { _y = y; }

    /// Get z value
    double z() const { return _z;}

    /// Set z value
    void setZ(double z) { _z = z;}

    /// @todo Uniform "coords" accessor across all Scatters: returning fixed-size tuple?

    // /// Get x,y,z value tuple
    // triple<double,double,double> xyz() const { return std::triple(_x, _y, _z); }

    /// Set x, y and z values
    void setXYZ(double x, double y, double z) { setX(x); setY(y); setZ(z); }

    // /// Set x and y values
    // void setXY(triple<double,double,double> xyz) { setX(xy.first); setY(xy.second); setZ(xy.third); }

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
    double xMin() const {
      return _x - _ex.first;
    }

    /// Get value plus positive x-error
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
    double yMin() const {
      return _y - _ey.first;
    }

    /// Get value plus positive y-error
    double yMax() const {
      return _y + _ey.second;
    }

    //@}


    /// @name z error accessors
    //@{

    /// Get z-error values
    const std::pair<double,double>& zErrs() const {
      return _ez;
    }

    /// Get negative z-error value
    double zErrMinus() const {
      return _ez.first;
    }

    /// Get positive z-error value
    double zErrPlus() const {
      return _ez.second;
    }

    /// Get average z-error value
    double zErrAvg() const {
      return (_ez.first + _ez.second)/2.0;
    }

    /// Set negative z error
    void setZErrMinus(double ezminus) {
      _ez.first = ezminus;
    }

    /// Set positive z error
    void setZErrPlus(double ezplus) {
      _ez.second = ezplus;
    }

    /// Set symmetric z error
    void setZErr(double ez) {
      setZErrMinus(ez);
      setZErrPlus(ez);
    }

    /// Set symmetric z error (alias)
    void setZErrs(double ez) {
      setZErr(ez);
    }

    /// Set asymmetric z error
    void setZErrs(double ezminus, double ezplus) {
      setZErrMinus(ezminus);
      setZErrPlus(ezplus);
    }

    /// Set asymmetric z error
    void setZErrs(const std::pair<double,double>& ez) {
      _ez = ez;
    }

    /// Get value minus negative z-error
    double zMin() const {
      return _z - _ez.first;
    }

    /// Get value plus positive z-error
    double zMax() const {
      return _z + _ez.second;
    }

    //@}


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


    /// Set z value and symmetric error
    void setZ(double z, double ez) {
      setZ(z);
      setZErrs(ez);
    }

    /// Set z value and asymmetric error
    void setZ(double z, double ezminus, double ezplus) {
      setZ(z);
      setZErrs(ezminus, ezplus);
    }

    /// Set z value and asymmetric error
    void setZ(double z, std::pair<double,double>& ez) {
      setZ(z);
      setZErrs(ez);
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

    /// Scaling of z axis
    void scaleZ(double scalez) {
      setZ(z()*scalez);
      setZErrs(zErrMinus()*scalez, zErrPlus()*scalez);
    }

    /// Scaling of all three axes
    void scaleXYZ(double scalex, double scaley, double scalez) {
      scaleX(scalex);
      scaleY(scaley);
      scaleZ(scalez);
    }

    /// Scaling of both axes
    /// @deprecated Use scaleXYZ
    void scale(double scalex, double scaley, double scalez) {
      scaleXYZ(scalex, scaley, scalez);
    }

    //@}


    /// @name Integer axis accessor equivalents
    //@{

    /// Get the point value for direction @a i
    double val(size_t i) const {
      switch (i) {
      case 1: return x();
      case 2: return y();
      case 3: return z();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set the point value for direction @a i
    void setVal(size_t i, double val) {
      switch (i) {
      case 1: setX(val);
      case 2: setY(val);
      case 3: setZ(val);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    /// Get error values for direction @a i
    const std::pair<double,double>& errs(size_t i) const {
      switch (i) {
      case 1: return xErrs();
      case 2: return yErrs();
      case 3: return zErrs();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Get negative error value for direction @a i
    double errMinus(size_t i) const {
      switch (i) {
      case 1: return xErrMinus();
      case 2: return yErrMinus();
      case 3: return zErrMinus();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Get positive error value for direction @a i
    double errPlus(size_t i) const {
      switch (i) {
      case 1: return xErrPlus();
      case 2: return yErrPlus();
      case 3: return zErrPlus();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Get average error value for direction @a i
    double errAvg(size_t i) const {
      switch (i) {
      case 1: return xErrAvg();
      case 2: return yErrAvg();
      case 3: return zErrAvg();
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    /// Set negative error for direction @a i
    void setErrMinus(size_t i, double eminus) {
      switch (i) {
      case 1: setXErrMinus(eminus);
      case 2: setYErrMinus(eminus);
      case 3: setZErrMinus(eminus);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set positive error for direction @a i
    void setErrPlus(size_t i, double eplus) {
      switch (i) {
      case 1: setXErrPlus(eplus);
      case 2: setYErrPlus(eplus);
      case 3: setZErrPlus(eplus);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    /// Set symmetric error for direction @a i
    void setErr(size_t i, double e) {
      switch (i) {
      case 1: setXErrs(e);
      case 2: setYErrs(e);
      case 3: setZErrs(e);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set asymmetric error for direction @a i
    void setErrs(size_t i, double eminus, double eplus) {
      switch (i) {
      case 1: setXErrs(eminus, eplus);
      case 2: setYErrs(eminus, eplus);
      case 3: setZErrs(eminus, eplus);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set asymmetric error for direction @a i
    void setErrs(size_t i, std::pair<double,double>& e) {
      switch (i) {
      case 1: setXErrs(e);
      case 2: setYErrs(e);
      case 3: setZErrs(e);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    /// Set value and symmetric error for direction @a i
    void set(size_t i, double val, double e) {
      switch (i) {
      case 1: setX(val, e);
      case 2: setY(val, e);
      case 3: setZ(val, e);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set value and asymmetric error for direction @a i
    void set(size_t i, double val, double eminus, double eplus) {
      switch (i) {
      case 1: setX(val, eminus, eplus);
      case 2: setY(val, eminus, eplus);
      case 3: setZ(val, eminus, eplus);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }
    /// Set value and asymmetric error for direction @a i
    void set(size_t i, double val, std::pair<double,double>& e) {
      switch (i) {
      case 1: setX(val, e);
      case 2: setY(val, e);
      case 3: setZ(val, e);
      default: throw RangeError("Invalid axis int, must be in range 1..dim");
      }
    }

    //@}


  protected:

    /// @name Value and error variables
    //@{

    double _x;
    double _y;
    double _z;
    std::pair<double,double> _ex;
    std::pair<double,double> _ey;
    std::pair<double,double> _ez;

    //@}

  };



  /// @name Comparison operators
  //@{

  /// Equality operator
  inline bool operator==(const  Point3D& a, const YODA::Point3D& b) {
    const bool same_val =  fuzzyEquals(a.x(), b.x()) && fuzzyEquals(a.y(), b.y());
    const bool same_eminus =  fuzzyEquals(a.xErrMinus(), b.xErrMinus()) &&
                              fuzzyEquals(a.yErrMinus(), b.yErrMinus());
    const bool same_eplus =  fuzzyEquals(a.xErrPlus(), b.xErrPlus()) &&
                             fuzzyEquals(a.yErrPlus(), b.yErrPlus());
    return same_val && same_eminus && same_eplus;
  }

  /// Inequality operator
  inline bool operator != (const  Point3D& a, const YODA::Point3D& b) {
    return !(a == b);
  }

  /// Less-than operator used to sort bins by x-first ordering
  inline bool operator < (const  Point3D& a, const YODA::Point3D& b) {
    if (! fuzzyEquals(a.x(), b.x())) {
      return a.x() < b.x();
    }
    if (!fuzzyEquals(a.y(), b.y())) {
      return a.y() < b.y();
    }
    if (! fuzzyEquals(a.xErrMinus(), b.xErrMinus())) {
      return a.xErrMinus() < b.xErrMinus();
    }
    if (!fuzzyEquals(a.yErrMinus(), b.yErrMinus())) {
      return a.yErrMinus() < b.yErrMinus();
    }
    if (! fuzzyEquals(a.xErrPlus(), b.xErrPlus())) {
      return a.xErrPlus() < b.xErrPlus();
    }
    if (!fuzzyEquals(a.yErrPlus(), b.yErrPlus())) {
      return a.yErrPlus() < b.yErrPlus();
    }
    return false;
  }

  /// Less-than-or-equals operator
  inline bool operator <= (const  Point3D& a, const YODA::Point3D& b) {
    if (a == b) return true;
    return a < b;
  }

  /// Greater-than operator
  inline bool operator > (const  Point3D& a, const YODA::Point3D& b) {
    return !(a <= b);
  }

  /// Greater-than-or-equals operator
  inline bool operator >= (const  Point3D& a, const YODA::Point3D& b) {
    return !(a < b);
  }

  //@}


}

#endif
