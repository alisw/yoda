#include "YODA/Scatter2D.h"
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"

namespace YODA {


  /// Make a Scatter2D representation of a Histo1D
  Scatter2D mkScatter(const Histo1D& h, bool usefocus) {
    Scatter2D rtn;
    for (const std::string& a : h.annotations())
      rtn.setAnnotation(a, h.annotation(a));
    rtn.setAnnotation("Type", h.type()); // might override the copied ones
    for (const HistoBin1D& b : h.bins()) {
      const double x = usefocus ? b.xFocus() : b.xMid();
      const double ex_m = x - b.xMin();
      const double ex_p = b.xMax() - x;

      double y;
      try {
        y = b.height();
      } catch (const Exception&) { // LowStatsError or WeightError
        y = std::numeric_limits<double>::quiet_NaN();
      }
      double ey;
      try {
        ey = b.heightErr();
      } catch (const Exception&) { // LowStatsError or WeightError
        ey = std::numeric_limits<double>::quiet_NaN();
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
    for (const std::string& a : p.annotations())
      rtn.setAnnotation(a, p.annotation(a));
    rtn.setAnnotation("Type", p.type());
    for (const ProfileBin1D& b : p.bins()) {
      const double x = usefocus ? b.xFocus() : b.xMid();
      const double ex_m = x - b.xMin();
      const double ex_p = b.xMax() - x;

      double y;
      try {
        y = b.mean();
      } catch (const Exception&) { // LowStatsError or WeightError
        y = std::numeric_limits<double>::quiet_NaN();
      }
      double ey;
      try {
        ey = usestddev ? b.stdDev() : b.stdErr(); ///< Control y-error scheme via usestddev arg
      } catch (const Exception&) { // LowStatsError or WeightError
        ey = std::numeric_limits<double>::quiet_NaN();
      }

      const Point2D pt(x, y, ex_m, ex_p, ey, ey);
      rtn.addPoint(pt);
    }
    assert(p.numBins() == rtn.numPoints());
    return rtn;
  }


}
