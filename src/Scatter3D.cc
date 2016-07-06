#include "YODA/Scatter3D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile2D.h"
#include "YODA/Exceptions.h"

namespace YODA {


  // // Add two scatters
  // Scatter3D add(const Scatter3D& , const Scatter3D& ) {
  //   /// @todo Implement
  //   throw Exception("Implement me!");
  //   Scatter3D tmp;
  //   return tmp;
  // }


  // // Subtract two scatters
  // Scatter3D subtract(const Scatter3D& , const Scatter3D& ) {
  //   /// @todo Implement
  //   throw Exception("Implement me!");
  //   Scatter3D tmp;
  //   return tmp;
  // }


  //////////////////////////


  Scatter3D mkScatter(const Histo2D& h, bool usefocus) {
    Scatter3D rtn;
    BOOST_FOREACH (const std::string& a, h.annotations())
      rtn.setAnnotation(a, h.annotation(a));
    rtn.setAnnotation("Type", h.type());
    for (size_t i = 0; i < h.numBins(); ++i) {
      const HistoBin2D& b = h.bin(i);

      const double x = usefocus ? b.xFocus() : b.xMid();
      const double exminus = x - b.xMin();
      const double explus = b.xMax() - x;

      const double y = usefocus ? b.yFocus() : b.yMid();
      const double eyminus = y - b.yMin();
      const double eyplus = b.yMax() - y;

      const double z = b.height();
      const double ez = b.heightErr();

      rtn.addPoint(x, y, z, exminus, explus, eyminus, eyplus, ez, ez);
    }

    return rtn;
  }


  Scatter3D mkScatter(const Profile2D& h, bool usefocus, bool usestddev) {
    Scatter3D rtn;
    BOOST_FOREACH (const std::string& a, h.annotations())
      rtn.setAnnotation(a, h.annotation(a));
    rtn.setAnnotation("Type", h.type());
    for (size_t i = 0; i < h.numBins(); ++i) {
      const ProfileBin2D& b = h.bin(i);

      const double x = usefocus ? b.xFocus() : b.xMid();
      const double exminus = x - b.xMin();
      const double explus = b.xMax() - x;

      const double y = usefocus ? b.yFocus() : b.yMid();
      const double eyminus = y - b.yMin();
      const double eyplus = b.yMax() - y;

      double z;
      try {
        z = b.mean();
      } catch (const LowStatsError& lse) {
        z = 0.0;
      }
      double ez;
      try {
        ez = usestddev ? b.stdDev() : b.stdErr(); ///< Control z-error scheme via usestddev arg
      } catch (const LowStatsError& lse) {
        ez = 0.0;
      }

      rtn.addPoint(x, y, z, exminus, explus, eyminus, eyplus, ez, ez);
    }

    return rtn;
  }


  // /// Divide two scatters
  // Scatter3D divide(const Scatter3D& numer, const Scatter3D& denom) {
  //   Scatter3D tmp;
  //   for (size_t i = 0; i < numer.numPoints(); ++i) {
  //     const Point3D& p1 = numer.point(i);
  //     const Point3D& p2 = denom.point(i);

  //     // Assemble the x value and error
  //     if (!fuzzyEquals(p1.xMin(), p2.xMin()) || !fuzzyEquals(p1.xMax(), p2.xMax()))
  //       throw BinningError("Point x 'bins' are not equivalent");
  //     // Use the midpoint of the "bin" for the new central x value, in the absence of better information
  //     const double x = (p1.xMin() + p1.xMax()) / 2.0;
  //     const double exminus = x - p1.xMin();
  //     const double explus  = p1.xMax() - x;

  //     // Assemble the y value and error
  //     if (!fuzzyEquals(p1.yMin(), p2.yMin()) || !fuzzyEquals(p1.yMax(), p2.yMax()))
  //       throw BinningError("Point x 'bins' are not equivalent");
  //     // Use the midpoint of the "bin" for the new central x value, in the absence of better information
  //     const double y = (p1.yMin() + p1.yMax()) / 2.0;
  //     const double eyminus = y - p1.yMin();
  //     const double eyplus  = p1.yMax() - y;

  //     double z = 0;
  //     double ez = 0;
  //     if (p1.z() == 0 || p2.z() == 0) {
  //       /// @todo find a nicer solution than setting the bin to zero
  //       //throw LowStatsError("Requested division of empty bin");
  //     } else {
  //       z = p1.z() / p2.z();
  //       /// @todo Generally deal with +/- errors separately
  //       ez = z * sqrt( sqr(p1.yErrAvg()/p1.z()) + sqr(p2.yErrAvg()/p2.z()) );
  //     }
  //     tmp.addPoint(x, y, z, exminus, explus, eyminus, eyplus, ez, ez);
  //   }
  //   assert(tmp.numPoints() == numer.numPoints());
  //   return tmp;
  // }


}
