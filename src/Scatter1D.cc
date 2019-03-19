#include "YODA/Scatter1D.h"
#include "YODA/Counter.h"
#include <sstream>

namespace YODA {


  /// Make a Scatter1D representation of a Histo1D
  Scatter1D mkScatter(const Counter& c) {
    Scatter1D rtn;
    for (const std::string& a : c.annotations())
      rtn.setAnnotation(a, c.annotation(a));
    rtn.setAnnotation("Type", c.type()); // might override the copied ones
    rtn.addPoint(c.val(), c.err());
    return rtn;
  }
  
  const std::vector<std::string> Scatter1D::variations() const  {
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
