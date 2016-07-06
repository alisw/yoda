#include "YODA/Scatter2D.h"
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"

namespace YODA {


  /// Make a Scatter2D representation of a Histo1D
  Scatter2D mkScatter(const Histo1D& h, bool usefocus) {
    Scatter2D rtn;
    BOOST_FOREACH (const std::string& a, h.annotations())
      rtn.setAnnotation(a, h.annotation(a));
    rtn.setAnnotation("Type", h.type()); // might override the copied ones
    BOOST_FOREACH (const HistoBin1D& b, h.bins()) {
      const double x = usefocus ? b.xFocus() : b.xMid();
      const double ex_m = x - b.xMin();
      const double ex_p = b.xMax() - x;

      double y;
      try {
        y = b.height();
      } catch (const Exception&) { // LowStatsError or WeightError
        y = 0;
      }
      double ey;
      try {
        ey = b.heightErr();
      } catch (const Exception&) { // LowStatsError or WeightError
        ey = 0;
      }

      const Point2D pt(x, y, ex_m, ex_p, ey, ey);
      rtn.addPoint(pt);
    }
    assert(h.numBins() == rtn.numPoints());
    return rtn;
  }


  /// Make a Scatter2D representation of a Profile1D
  Scatter2D mkScatter(const Profile1D& p, bool usefocus, bool usestddev) {
    Scatter2D rtn;
    BOOST_FOREACH (const std::string& a, p.annotations())
      rtn.setAnnotation(a, p.annotation(a));
    rtn.setAnnotation("Type", p.type());
    BOOST_FOREACH (const ProfileBin1D& b, p.bins()) {
      const double x = usefocus ? b.xFocus() : b.xMid();
      const double ex_m = x - b.xMin();
      const double ex_p = b.xMax() - x;

      double y;
      try {
        y = b.mean();
      } catch (const Exception&) { // LowStatsError or WeightError
        y = 0.0;
      }
      double ey;
      try {
        ey = usestddev ? b.stdDev() : b.stdErr(); ///< Control y-error scheme via usestddev arg
      } catch (const Exception&) { // LowStatsError or WeightError
        ey = 0.0;
      }

      const Point2D pt(x, y, ex_m, ex_p, ey, ey);
      rtn.addPoint(pt);
    }
    assert(p.numBins() == rtn.numPoints());
    return rtn;
  }


  ////////////////////////////////////////


  // /// @todo Lots of boilerplate shared between these three functions, but I can't think of a
  // ///   C++ way to do it better, since functors for value and error combination are *so* heavy.


  // // Add two scatters
  // /// @todo Does this even make sense?
  // Scatter2D add(const Scatter2D& first, const Scatter2D& second) {
  //   Scatter2D tmp;
  //   for (size_t i = 0; i < first.numPoints(); ++i) {
  //     const Point2D& p1 = first.point(i);
  //     const Point2D& p2 = second.point(i);
  //     if (!fuzzyEquals(p1.xMin(), p2.xMin()) || !fuzzyEquals(p1.xMax(), p2.xMax()))
  //       throw BinningError("Point x binnings are not equivalent for " + first.path() + " + " + second.path());
  //     // Use the midpoint of the "bin" for the new central x value, in the absence of better information
  //     const double x = (p1.xMin() + p1.xMax()) / 2.0;
  //     const double exminus = x - p1.xMin();
  //     const double explus  = p1.xMax() - x;
  //     //
  //     const double y = p1.y() + p2.y();
  //     /// @todo Deal with +/- errors separately?
  //     const double ey = sqrt( sqr(p1.yErrAvg()) + sqr(p2.yErrAvg()) );
  //     tmp.addPoint(x, y, exminus, explus, ey, ey);
  //   }
  //   assert(tmp.numPoints() == first.numPoints());
  //   return tmp;
  // }


  // // Subtract two scatters
  // /// @todo Does this even make sense?
  // Scatter2D subtract(const Scatter2D& first, const Scatter2D& second) {
  //   Scatter2D tmp;
  //   for (size_t i = 0; i < first.numPoints(); ++i) {
  //     const Point2D& p1 = first.point(i);
  //     const Point2D& p2 = second.point(i);
  //     if (!fuzzyEquals(p1.xMin(), p2.xMin()) || !fuzzyEquals(p1.xMax(), p2.xMax()))
  //       throw BinningError("Point x binnings are not equivalent in " + first.path() + " - " + second.path());
  //     // Use the midpoint of the "bin" for the new central x value, in the absence of better information
  //     const double x = (p1.xMin() + p1.xMax()) / 2.0;
  //     const double exminus = x - p1.xMin();
  //     const double explus  = p1.xMax() - x;
  //     //
  //     const double y = p1.y() - p2.y();
  //     /// @todo Deal with +/- errors separately?
  //     const double ey = sqrt( sqr(p1.yErrAvg()) + sqr(p2.yErrAvg()) );
  //     tmp.addPoint(x, y, exminus, explus, ey, ey);
  //   }
  //   assert(tmp.numPoints() == first.numPoints());
  //   return tmp;
  // }


  // // Divide two scatters
  // /// @todo Need to rework this: should be able to divide matching pairs of points along either axis
  // /// @todo In general any pairing of points should be able to have any binary operation applied to it...
  // Scatter2D divide(const Scatter2D& numer, const Scatter2D& denom) {
  //   Scatter2D tmp;

  //   // for (size_t i = 0; i < numer.numPoints(); ++i) {
  //   //   std::cout << numer.point(i).xMin() << " ";
  //   //   std::cout << numer.point(i).xMax() << " ";
  //   // }
  //   // std::cout << std::endl;
  //   // for (size_t i = 0; i < denom.numPoints(); ++i) {
  //   //   std::cout << denom.point(i).xMin() << " ";
  //   //   std::cout << denom.point(i).xMax() << " ";
  //   // }
  //   // std::cout << std::endl;

  //   for (size_t i = 0; i < numer.numPoints(); ++i) {
  //     const Point2D& p1 = numer.point(i);
  //     const Point2D& p2 = denom.point(i);

  //     // Assemble the x value and error
  //     // std::cout << p1.xMin() << " vs " << p2.xMin() << " and " << p1.xMax() << " vs " << p2.xMax() << std::endl;
  //     if (!fuzzyEquals(p1.xMin(), p2.xMin()) || !fuzzyEquals(p1.xMax(), p2.xMax()))
  //       throw BinningError("Point x binnings are not equivalent in " + numer.path() + " / " + denom.path());

  //     // Use the midpoint of the "bin" for the new central x value, in the absence of better information
  //     const double x = (p1.xMin() + p1.xMax()) / 2.0;
  //     const double exminus = x - p1.xMin();
  //     const double explus  = p1.xMax() - x;

  //     // Assemble the y value and error
  //     double y = 0;
  //     double ey = 0;
  //     if (p2.y() == 0 || (p1.y() == 0 && p1.yErrAvg() != 0)) { //< @todo Why only if p1.yErrAvg() != 0, if p1.y == 0?
  //       /// @todo Provide optional alt behaviours to fill with NaN or remove the invalid point or throw
  //       /// @todo Don't throw here: set a flag and throw after all bins have been handled.
  //       // throw LowStatsError("Requested division of empty bin");
  //     } else {
  //       y = p1.y() / p2.y();
  //       /// @todo Is this the exact error treatment for all (uncorrelated) cases? Behaviour around 0? +1 and -1 fills?
  //       const double relerr_1 = p1.yErrAvg() != 0 ? p1.yErrAvg()/p1.y() : 0;
  //       const double relerr_2 = p2.yErrAvg() != 0 ? p2.yErrAvg()/p2.y() : 0;
  //       ey = y * sqrt(sqr(relerr_1) + sqr(relerr_2));
  //     }
  //     /// Deal with +/- errors separately, inverted for the denominator contributions:
  //     /// @todo check correctness with different signed numerator and denominator.
  //     //const double eyplus = y * sqrt( sqr(p1.yErrPlus()/p1.y()) + sqr(p2.yErrMinus()/p2.y()) );
  //     //const double eyminus = y * sqrt( sqr(p1.yErrMinus()/p1.y()) + sqr(p2.yErrPlus()/p2.y()) );
  //     tmp.addPoint(x, y, exminus, explus, ey, ey);
  //   }
  //   assert(tmp.numPoints() == numer.numPoints());
  //   return tmp;
  // }


}
