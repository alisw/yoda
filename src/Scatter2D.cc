#include "YODA/Scatter2D.h"
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"
#include <sstream>
#include "yaml-cpp/yaml.h"
#ifdef YAML_NAMESPACE
#define YAML YAML_NAMESPACE
#endif

namespace YODA {


  /// Make a Scatter2D representation of a Histo1D
  Scatter2D mkScatter(const Histo1D& h, bool usefocus, bool binwidthdiv) {
    Scatter2D rtn;
    for (const std::string& a : h.annotations()) rtn.setAnnotation(a, h.annotation(a));
    rtn.setAnnotation("Type", h.type()); // might override the copied ones

    for (const HistoBin1D& b : h.bins()) {
      const double x = usefocus ? b.xFocus() : b.xMid();
      const double ex_m = x - b.xMin();
      const double ex_p = b.xMax() - x;

      double y;
      try {
        y = b.sumW();
      } catch (const Exception&) { // LowStatsError or WeightError
        y = std::numeric_limits<double>::quiet_NaN();
      }
      if (binwidthdiv) y /= b.xWidth();
      const double ey = b.relErr() * y;

      // Attach the point to its parent
      Point2D pt(x, y, ex_m, ex_p, ey, ey);
      pt.setParentAO(&rtn);
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

      //const Point2D pt(x, y, ex_m, ex_p, ey, ey);
      Point2D pt(x, y, ex_m, ex_p, ey, ey);
      pt.setParentAO(&rtn);
      rtn.addPoint(pt);
    }
    assert(p.numBins() == rtn.numPoints());
    return rtn;
  }

  // retrieve variations from annoation, parse them as YAML, and update the points
  void Scatter2D::parseVariations()  {
    if (this-> _variationsParsed) { return; }
    if (!(this->hasAnnotation("ErrorBreakdown"))) { return; }
    YAML::Node errorBreakdown;
    errorBreakdown = YAML::Load(this->annotation("ErrorBreakdown"));

    if (errorBreakdown.size()) {
      for (unsigned int thisPointIndex=0 ; thisPointIndex< this->numPoints() ; ++thisPointIndex){
        Point2D &thispoint = this->_points[thisPointIndex];
        YAML::Node variations = errorBreakdown[thisPointIndex];
        for (const auto& variation : variations) {
          const std::string variationName = variation.first.as<std::string>();
          double eyp = variation.second["up"].as<double>();
          double eym = variation.second["dn"].as<double>();
          thispoint.setYErrs(eym,eyp,variationName);
        }
      }
      this-> _variationsParsed =true;
    }
  }

  const std::vector<std::string> Scatter2D::variations() const  {
    std::vector<std::string> vecVariations;
    for (auto &point : this->_points){
      for (auto &it : point.errMap()){
        //if the variation is not already in the vector, add it !
        if (std::find(vecVariations.begin(), vecVariations.end(), it.first) == vecVariations.end()){
          vecVariations.push_back(it.first);
        }
      }
    }
    return vecVariations;
  }


  std::vector<std::vector<double> > Scatter2D::covarianceMatrix( bool ignoreOffDiagonalTerms)   {
    int nPoints= this->numPoints();
    //double covM[nPoints][nPoints] ={};
    std::vector<std::vector<double> > covM;


    // initialose cov matrix to be the right shape!
    for (int i=0; i<nPoints ; i++) {
      std::vector< double> row;
      row.resize(nPoints);
      covM.push_back(row);
    }

    // case where only have nominal, ie total uncertainty, labelled "" (empty string)
    if (this->variations().size()==1) {
      for (int i=0; i<nPoints ; i++) {
        covM[i][i]= pow(((this->_points[i].yErrs().first+this->_points[i].yErrs().second)/2),2);
        if (covM[i][i]==0 )  covM[i][i]=1;
      }
      return covM;
    }
    //more interesting case where we actually have some uncertainty breakdown!
    auto  systList= this->variations();
    for (auto sname : systList){
      if (sname.length()==0) continue;
      std::vector< double> systErrs;
      systErrs.resize(nPoints);
      for (int i=0; i<nPoints ; i++) {
        auto point = this->_points[i];
        try {
          auto variations=point.errMap().at(sname);
          systErrs[i]=(fabs(variations.first)+fabs(variations.second))*0.5 ;//up/dn are symmetrized since this method can't handle asymmetric errors
        } catch (const std::exception& e) { // Missing bin.
          systErrs[i]=0.0;
        }
      }
      if (ignoreOffDiagonalTerms ||  sname.find("stat") != std::string::npos ||  sname.find("uncor") != std::string::npos){
        for (int i=0; i<nPoints ; i++) {
          covM[i][i] += systErrs[i]*systErrs[i]; // just the diagonal, bins are considered uncorrelated
        }
      }else{
        for (int i=0; i<nPoints ; i++) {
          for (int j=0; j<nPoints ; j++) {
            covM[i][j] += systErrs[i]*systErrs[j];
          }
        }
      }
    }
    return covM;
  }

}
