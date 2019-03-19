#include "YODA/Scatter3D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile2D.h"
#include "YODA/Exceptions.h"
#include <sstream>

namespace YODA {


  Scatter3D mkScatter(const Histo2D& h, bool usefocus) {
    Scatter3D rtn;
    for (const std::string& a : h.annotations())
      rtn.setAnnotation(a, h.annotation(a));
    rtn.setAnnotation("Type", h.type());
    for (size_t i = 0; i < h.numBins(); ++i) {
      const HistoBin2D& b = h.bin(i);

      /// SAME FOR ALL 2D BINS

      double x = b.xMid();
      if (usefocus) {
        try {
          x = b.xFocus();
        } catch (const LowStatsError& lse) {
          x = b.xMid();
        }
      }
      const double exminus = x - b.xMin();
      const double explus = b.xMax() - x;

      double y = b.yMid();
      if (usefocus) {
        try {
          y = b.yFocus();
        } catch (const LowStatsError& lse) {
          y = b.yMid();
        }
      }
      const double eyminus = y - b.yMin();
      const double eyplus = b.yMax() - y;

      /// END SAME FOR ALL 2D BINS

      const double z = b.height();
      const double ez = b.heightErr();

      rtn.addPoint(x, y, z, exminus, explus, eyminus, eyplus, ez, ez);
    }

    return rtn;
  }


  Scatter3D mkScatter(const Profile2D& h, bool usefocus, bool usestddev) {
    Scatter3D rtn;
    for (const std::string& a : h.annotations())
      rtn.setAnnotation(a, h.annotation(a));
    rtn.setAnnotation("Type", h.type());
    for (size_t i = 0; i < h.numBins(); ++i) {
      const ProfileBin2D& b = h.bin(i);

      /// SAME FOR ALL 2D BINS

      double x = b.xMid();
      if (usefocus) {
        try {
          x = b.xFocus();
        } catch (const LowStatsError& lse) {
          x = b.xMid();
        }
      }
      const double exminus = x - b.xMin();
      const double explus = b.xMax() - x;

      double y = b.yMid();
      if (usefocus) {
        try {
          y = b.yFocus();
        } catch (const LowStatsError& lse) {
          y = b.yMid();
        }
      }
      const double eyminus = y - b.yMin();
      const double eyplus = b.yMax() - y;

      /// END SAME FOR ALL 2D BINS


      double z;
      try {
        z = b.mean();
      } catch (const LowStatsError& lse) {
        z = std::numeric_limits<double>::quiet_NaN();
      }
      double ez;
      try {
        ez = usestddev ? b.stdDev() : b.stdErr(); ///< Control z-error scheme via usestddev arg
      } catch (const LowStatsError& lse) {
        ez = std::numeric_limits<double>::quiet_NaN();
      }

      rtn.addPoint(x, y, z, exminus, explus, eyminus, eyplus, ez, ez);
    }

    return rtn;
  }
  
  
  const std::vector<std::string> Scatter3D::variations() const  {
    std::vector<std::string> vecvariations;
    for (auto &point : this->_points){
      for (auto &it : point.errMap()){
        //if the variation is not already in the vector, add it !
        if (std::find(vecvariations.begin(), vecvariations.end(), it.first) == vecvariations.end()){
          vecvariations.push_back(it.first);
        }
      }
    }
    return vecvariations;
  }

}
