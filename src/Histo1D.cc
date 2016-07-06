// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"
#include "YODA/Scatter2D.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  void Histo1D::fill(double x, double weight) {
    if ( std::isnan(x) ) throw RangeError("X is NaN");

    // Fill the overall distribution
    _axis.totalDbn().fill(x, weight);

    // Fill the bins and overflows
    /// Unify this with Profile1D's version, when binning and inheritance are reworked
    if (inRange(x, _axis.xMin(), _axis.xMax())) {
      try {
        /// @todo Replace try block with a check that there is a bin at x
        binAt(x).fill(x, weight);
      } catch (const RangeError& re) {    }
    } else if (x < _axis.xMin()) {
      _axis.underflow().fill(x, weight);
    } else if (x >= _axis.xMax()) {
      _axis.overflow().fill(x, weight);
    }

    // Lock the axis now that a fill has happened
    _axis._setLock(true);
  }


  void Histo1D::fillBin(size_t i, double weight) {
    fill(bin(i).xMid(), weight);
  }



  /////////////// COMMON TO ALL BINNED

  unsigned long Histo1D::numEntries(bool includeoverflows) const {
    if (includeoverflows) return totalDbn().numEntries();
    unsigned long n = 0;
    BOOST_FOREACH (const Bin& b, bins()) n += b.numEntries();
    return n;
  }


  double Histo1D::effNumEntries(bool includeoverflows) const {
    if (includeoverflows) return totalDbn().effNumEntries();
    double n = 0;
    BOOST_FOREACH (const Bin& b, bins()) n += b.effNumEntries();
    return n;
  }


  double Histo1D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW();
    double sumw = 0;
    BOOST_FOREACH (const Bin& b, bins()) sumw += b.sumW();
    return sumw;
  }


  double Histo1D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw2 = 0;
    BOOST_FOREACH (const Bin& b, bins()) sumw2 += b.sumW2();
    return sumw2;
  }

  // ^^^^^^^^^^^^^


  double Histo1D::xMean(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xMean();
    Dbn1D dbn;
    BOOST_FOREACH (const HistoBin1D& b, bins()) dbn += b.dbn();
    return dbn.xMean();
  }


  double Histo1D::xVariance(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xVariance();
    Dbn1D dbn;
    BOOST_FOREACH (const HistoBin1D& b, bins()) dbn += b.dbn();
    return dbn.xVariance();
  }


  double Histo1D::xStdErr(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xStdErr();
    Dbn1D dbn;
    BOOST_FOREACH (const HistoBin1D& b, bins()) dbn += b.dbn();
    return dbn.xStdErr();
  }


  double Histo1D::xRMS(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().xRMS();
    Dbn1D dbn;
    BOOST_FOREACH (const HistoBin1D& b, bins()) dbn += b.dbn();
    return dbn.xRMS();
  }


  ////////////////////////////////////////


  /// Copy constructor with optional new path
  Histo1D::Histo1D(const Histo1D& h, const std::string& path)
    : AnalysisObject("Histo1D", (path.size() == 0) ? h.path() : path, h, h.title())
  {
    _axis = h._axis;
  }


  /// Constructor from a Scatter2D's binning, with optional new path
  Histo1D::Histo1D(const Scatter2D& s, const std::string& path)
    : AnalysisObject("Histo1D", (path.size() == 0) ? s.path() : path, s, s.title())
  {
    std::vector<HistoBin1D> bins;
    BOOST_FOREACH (const Scatter2D::Point& p, s.points()) {
      bins.push_back(HistoBin1D(p.xMin(), p.xMax()));
    }
    _axis = Histo1DAxis(bins);
  }


  /// Constructor from a Profile1D's binning, with optional new path
  Histo1D::Histo1D(const Profile1D& p, const std::string& path)
    : AnalysisObject("Histo1D", (path.size() == 0) ? p.path() : path, p, p.title())
  {
    std::vector<HistoBin1D> bins;
    BOOST_FOREACH (const ProfileBin1D& b, p.bins()) {
      bins.push_back(HistoBin1D(b.xMin(), b.xMax()));
    }
    _axis = Histo1DAxis(bins);
  }


  ////////////////////////////////////////


  // Divide two histograms
  Scatter2D divide(const Histo1D& numer, const Histo1D& denom) {
    Scatter2D rtn;

    for (size_t i = 0; i < numer.numBins(); ++i) {
      const HistoBin1D& b1 = numer.bin(i);
      const HistoBin1D& b2 = denom.bin(i);

      /// @todo Create a compatibleBinning function? Or just compare vectors of edges().
      if (!fuzzyEquals(b1.xMin(), b2.xMin()) || !fuzzyEquals(b1.xMax(), b2.xMax()))
        throw BinningError("x binnings are not equivalent in " + numer.path() + " / " + denom.path());

      // Assemble the x value and error
      // Use the midpoint of the "bin" for the new central x value, in the absence of better information
      const double x = b1.xMid();
      const double exminus = x - b1.xMin();
      const double explus  = b1.xMax() - x;

      // Assemble the y value and error
      double y = 0;
      double ey = 0;
      if (b2.height() == 0 || (b1.height() == 0 && b1.heightErr() != 0)) { ///< @todo Ok?
        /// @todo Provide optional alt behaviours to fill with NaN or remove the invalid point or throw
        /// @todo Don't throw here: set a flag and throw after all bins have been handled.
        // throw LowStatsError("Requested division of empty bin");
      } else {
        y = b1.height() / b2.height();
        /// @todo Is this the exact error treatment for all (uncorrelated) cases? Behaviour around 0? +1 and -1 fills?
        const double relerr_1 = b1.heightErr() != 0 ? b1.relErr() : 0;
        const double relerr_2 = b2.heightErr() != 0 ? b2.relErr() : 0;
        ey = y * sqrt(sqr(relerr_1) + sqr(relerr_2));
      }
      /// Deal with +/- errors separately, inverted for the denominator contributions:
      /// @todo check correctness with different signed numerator and denominator.
      //const double eyplus = y * sqrt( sqr(p1.yErrPlus()/p1.y()) + sqr(p2.yErrMinus()/p2.y()) );
      //const double eyminus = y * sqrt( sqr(p1.yErrMinus()/p1.y()) + sqr(p2.yErrPlus()/p2.y()) );
      rtn.addPoint(x, y, exminus, explus, ey, ey);
    }

    assert(rtn.numPoints() == numer.numBins());
    return rtn;
  }


  // Calculate a histogrammed efficiency ratio of two histograms
  Scatter2D efficiency(const Histo1D& accepted, const Histo1D& total) {
    Scatter2D tmp = divide(accepted, total);
    for (size_t i = 0; i < accepted.numBins(); ++i) {
      const HistoBin1D& b_acc = accepted.bin(i);
      const HistoBin1D& b_tot = total.bin(i);
      Point2D& point = tmp.point(i);

      /// BEGIN DIMENSIONALITY-INDEPENDENT BIT TO SHARE WITH H2

      // Check that the numerator is consistent with being a subset of the denominator (NOT effNumEntries here!)
      if (b_acc.numEntries() > b_tot.numEntries() || b_acc.sumW() > b_tot.sumW())
        throw UserError("Attempt to calculate an efficiency when the numerator is not a subset of the denominator");

      // If no entries on the denominator, set eff = err = 0 and move to the next bin
      /// @todo Provide optional alt behaviours to fill with NaN or remove the invalid point, or...
      /// @todo Or throw a LowStatsError exception if h.effNumEntries() (or sumW()?) == 0?
      double eff = 0, err = 0;
      if (b_tot.sumW() != 0) {
        // Calculate the values and errors
        // const double eff = b_acc.effNumEntries() / b_tot.effNumEntries();
        // const double ey = sqrt( b_acc.effNumEntries() * (1 - b_acc.effNumEntries()/b_tot.effNumEntries()) ) / b_tot.effNumEntries();
        eff = b_acc.sumW() / b_tot.sumW(); //< Actually this is already calculated by the division...
        err = sqrt(abs( ((1-2*eff)*b_acc.sumW2() + sqr(eff)*b_tot.sumW2()) / sqr(b_tot.sumW()) ));
        // assert(point.y() == eff); //< @todo Correct? So we don't need to reset the eff on the next line?
      }

      /// END DIMENSIONALITY-INDEPENDENT BIT TO SHARE WITH H2

      point.setY(eff, err);
    }
    return tmp;
  }


  // Convert a Histo1D to a Scatter2D representing the integral of the histogram
  Scatter2D toIntegralHisto(const Histo1D& h, bool includeunderflow) {
    /// @todo Check that the histogram binning has no gaps, otherwise throw a BinningError
    Scatter2D tmp = mkScatter(h);
    double integral = includeunderflow ? h.underflow().sumW() : 0.0;
    for (size_t i = 0; i < h.numBins(); ++i) {
      Point2D& point = tmp.point(i);
      integral += h.bin(i).sumW();
      const double err = sqrt(integral); //< @todo Should be sqrt(sumW2)? Or more complex, cf. Simon etc.?
      point.setY(integral, err);
    }
    return tmp;
  }


  Scatter2D toIntegralEfficiencyHisto(const Histo1D& h, bool includeunderflow, bool includeoverflow) {
    Scatter2D rtn = toIntegralHisto(h, includeunderflow);
    const double integral = h.integral() - (includeoverflow ? 0 : h.overflow().sumW());
    /// @todo Should the total integral *error* be sqrt(sumW2)? Or more complex, cf. Simon etc.?
    const double integral_err = sqrt(integral);

    // If the integral is empty, the (integrated) efficiency values may as well all be zero, so return here
    /// @todo Or throw a LowStatsError exception if h.effNumEntries() == 0?
    /// @todo Provide optional alt behaviours
    /// @todo Need to check that bins are all positive? Integral could be zero due to large +ve/-ve in different bins :O
    if (integral == 0) return rtn;

    // Normalize and compute binomial errors
    BOOST_FOREACH (Point2D& p, rtn.points()) {
      const double eff = p.y() / integral;
      /// @todo Should the total integral error be sqrt(sumW2)? Or more complex, cf. Simon etc.?
      const double ey = sqrt(abs( ((1-2*eff)*sqr(p.y()/p.yErrAvg()) + sqr(eff)*sqr(integral_err)) / sqr(integral) ));
      p.setY(eff, ey);
    }

    return rtn;
  }


}
