#include "YODA/Scatter1D.h"
#include "YODA/Counter.h"

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


}
