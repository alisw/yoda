#include "YODA/Scatter3D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile2D.h"
#include "YODA/Exceptions.h"
#include <sstream>
#include "yaml-cpp/yaml.h"
#ifdef YAML_NAMESPACE
#define YAML YAML_NAMESPACE
#endif

namespace YODA {


  Scatter3D mkScatter(const Histo2D& h, bool usefocus, bool binareadiv) {
    Scatter3D rtn;
    for (const std::string& a : h.annotations()) rtn.setAnnotation(a, h.annotation(a));
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

      double z;
      try {
        z = b.sumW();
      } catch (const Exception&) { // LowStatsError or WeightError
        z = std::numeric_limits<double>::quiet_NaN();
      }
      if (binareadiv) z /= b.xWidth()*b.yWidth();
      const double ez = b.relErr() * z;


      Point3D pt(x, y, z, exminus, explus, eyminus, eyplus, ez, ez);
      pt.setParentAO(&rtn);
      rtn.addPoint(pt);
    }

    assert(h.numBins() == rtn.numPoints());
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

  void Scatter3D::parseVariations()   {
    if (this-> _variationsParsed) { return; }
    if (!(this->hasAnnotation("ErrorBreakdown"))) { return;}
    YAML::Node errorBreakdown;
    errorBreakdown = YAML::Load(this->annotation("ErrorBreakdown"));
    if (errorBreakdown.size()) {
      for (unsigned int thisPointIndex=0 ; thisPointIndex< this->numPoints() ; ++thisPointIndex){
        Point3D &thispoint = this->_points[thisPointIndex];
        YAML::Node variations = errorBreakdown[thisPointIndex];
        for (const auto& variation : variations) {
          const std::string variationName = variation.first.as<std::string>();
          double eyp = variation.second["up"].as<double>();
          double eym = variation.second["dn"].as<double>();
          thispoint.setZErrs(eym,eyp,variationName);
        }
      }
      this-> _variationsParsed =true;
    }
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
