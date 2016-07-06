#include "YODA/Scatter1D.h"
#include "YODA/Counter.h"

namespace YODA {


  /// Make a Scatter1D representation of a Histo1D
  Scatter1D mkScatter(const Counter& c) {
    Scatter1D rtn;
    BOOST_FOREACH (const std::string& a, c.annotations())
      rtn.setAnnotation(a, c.annotation(a));
    rtn.setAnnotation("Type", c.type()); // might override the copied ones
    rtn.addPoint(c.val(), c.err());
    return rtn;
  }


  ////////////////////////////////////////


  /// @todo Lots of boilerplate shared between these three functions, but I can't think of a
  ///   C++ way to do it better, since functors for value and error combination are *so* heavy.


  // // Add two scatters
  // /// @todo Does this even make sense?
  // Scatter1D add(const Scatter1D& first, const Scatter1D& second) {
  //   Scatter1D tmp;
  //   for (size_t i = 0; i < first.numPoints(); ++i) {
  //     const Point1D& p1 = first.point(i);
  //     const Point1D& p2 = second.point(i);
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
  // Scatter1D subtract(const Scatter1D& first, const Scatter1D& second) {
  //   Scatter1D tmp;
  //   for (size_t i = 0; i < first.numPoints(); ++i) {
  //     const Point1D& p1 = first.point(i);
  //     const Point1D& p2 = second.point(i);
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
  // Scatter1D divide(const Scatter1D& numer, const Scatter1D& denom) {
  //   Scatter1D tmp;

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
  //     const Point1D& p1 = numer.point(i);
  //     const Point1D& p2 = denom.point(i);

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
