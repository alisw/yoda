#include "YODA/Scatter1D.h"
#include "YODA/Counter.h"
#include <sstream>
#include "yaml-cpp/yaml.h"
#ifdef YAML_NAMESPACE
#define YAML YAML_NAMESPACE
#endif

namespace YODA {


  /// Make a Scatter1D representation of a Histo1D
  Scatter1D mkScatter(const Counter& c) {
    Scatter1D rtn;
    for (const std::string& a : c.annotations())
      rtn.setAnnotation(a, c.annotation(a));
    rtn.setAnnotation("Type", c.type()); // might override the copied ones
    Point1D pt(c.val(), c.err());
    pt.setParent(&rtn);
    rtn.addPoint(pt);
    return rtn;
  }


  void Scatter1D::parseVariations() {
    if (this->_variationsParsed) { return;}
    if (!(this->hasAnnotation("ErrorBreakdown"))) { return; }
    YAML::Node errorBreakdown;
    errorBreakdown = YAML::Load(this->annotation("ErrorBreakdown"));

    if (errorBreakdown.size()) {
      for (size_t thisPointIndex = 0; thisPointIndex < this->numPoints(); ++thisPointIndex) {
        Point1D &thispoint = this->_points[thisPointIndex];
        YAML::Node variations = errorBreakdown[thisPointIndex];
        for (const auto& variation : variations) {
          const std::string variationName = variation.first.as<std::string>();
          double eyp = variation.second["up"].as<double>();
          double eym = variation.second["dn"].as<double>();
          thispoint.setXErrs(eym,eyp,variationName);
        }
      }
      this-> _variationsParsed =true;
    }
  }


  /// @todo Reduce duplication between Scatter types
  std::vector<std::string> Scatter1D::variations() const  {
    /// @todo Auto-run parseVariations? Why expose the machinery to the user?
    std::vector<std::string> vecvariations;
    for (auto& point : this->_points) {
      for (auto& it : point.errMap()) {
        // if the variation is not already in the vector, add it!
        if (std::find(vecvariations.begin(), vecvariations.end(), it.first) == vecvariations.end()) {
          vecvariations.push_back(it.first);
        }
      }
    }
    return vecvariations;
  }

}
