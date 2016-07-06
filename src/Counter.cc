// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/Counter.h"

#include <cmath>
#include <iostream>
using namespace std;

namespace YODA {


  /// Copy constructor with optional new path
  Counter::Counter(const Counter& c, const std::string& path)
    : AnalysisObject("Counter", (path.size() == 0) ? c.path() : path, c, c.title())
  {
    _dbn = c._dbn;
  }


  // Divide two counters
  Scatter1D divide(const Counter& numer, const Counter& denom) {
    Scatter1D rtn;
    if (denom.val() != 0) {
      const double val = numer.val() / denom.val();
      const double err = val * add_quad(numer.relErr(), denom.relErr());
      rtn.addPoint(val, err);
    } else {
      rtn.addPoint(0, 0);
    }
    return rtn;
  }


  // Calculate a histogrammed efficiency ratio of two histograms
  Scatter1D efficiency(const Counter& accepted, const Counter& total) {
    Scatter1D tmp = divide(accepted, total);
    assert(tmp.numPoints() == 1);

    /// BEGIN DIMENSIONALITY-INDEPENDENT BIT TO SHARE WITH H1

    // Check that the numerator is consistent with being a subset of the denominator (NOT effNumEntries here!)
    if (accepted.numEntries() > total.numEntries() || accepted.sumW() > total.sumW())
      throw UserError("Attempt to calculate an efficiency when the numerator is not a subset of the denominator");

    // If no entries on the denominator, set eff = err = 0 and move to the next bin
    /// @todo Provide optional alt behaviours to fill with NaN or remove the invalid point, or...
    /// @todo Or throw a LowStatsError exception if h.effNumEntries() (or sumW()?) == 0?
    double eff = 0, err = 0;
    if (total.sumW() != 0) {
      // Calculate the values and errors
      // const double eff = b_acc.effNumEntries() / b_tot.effNumEntries();
      // const double ey = sqrt( b_acc.effNumEntries() * (1 - b_acc.effNumEntries()/b_tot.effNumEntries()) ) / b_tot.effNumEntries();
      eff = accepted.sumW() / total.sumW(); //< Actually this is already calculated by the division...
      err = sqrt(abs( ((1-2*eff)*accepted.sumW2() + sqr(eff)*total.sumW2()) / sqr(total.sumW()) ));
      // assert(point.y() == eff); //< @todo Correct? So we don't need to reset the eff on the next line?
    }

    /// END DIMENSIONALITY-INDEPENDENT BIT TO SHARE WITH H1

    tmp.point(0).setX(eff, err);
    return tmp;
  }


}
