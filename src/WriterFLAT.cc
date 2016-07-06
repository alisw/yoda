// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/WriterFLAT.h"

//#include "YODA/Counter.h"
#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Profile2D.h"
#include "YODA/Scatter1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Scatter3D.h"

#include <iostream>
#include <iomanip>

using namespace std;

namespace YODA {


  void WriterFLAT::writeHeader(std::ostream& os) {
    os << flush;
  }


  void WriterFLAT::writeFooter(std::ostream& os) {
    os << flush;
  }


  void WriterFLAT::_writeAnnotations(std::ostream& os, const AnalysisObject& ao) {
    os << scientific << setprecision(_precision);
    BOOST_FOREACH (const string& a, ao.annotations()) {
      if (a.empty()) continue;
      if (a == "Type") continue;
      /// @todo Should write out floating point annotations as scientific notation...
      os << a << "=" << ao.annotation(a) << "\n";
    }
  }


  void WriterFLAT::writeCounter(std::ostream& os, const Counter& c) {
    ios_base::fmtflags oldflags = os.flags();
    os << scientific << showpoint << setprecision(_precision);

    os << "# BEGIN COUNTER " << c.path() << "\n";
    _writeAnnotations(os, c);
    os << "# value\t error\n";
    os << c.val() << "\t" << c.err() << "\n";
    os << "# END COUNTER\n\n";

    os << flush;
    os.flags(oldflags);
  }


  void WriterFLAT::writeHisto1D(std::ostream& os, const Histo1D& h) {
    Scatter2D tmp = mkScatter(h);
    tmp.setAnnotation("Type", "Histo1D");
    writeScatter2D(os, tmp);
  }


  void WriterFLAT::writeHisto2D(std::ostream& os, const Histo2D& h) {
    Scatter3D tmp = mkScatter(h);
    tmp.setAnnotation("Type", "Histo2D");
    writeScatter3D(os, tmp);
  }


  void WriterFLAT::writeProfile1D(std::ostream& os, const Profile1D& p) {
    Scatter2D tmp = mkScatter(p);
    tmp.setAnnotation("Type", "Profile1D");
    writeScatter2D(os, tmp);
  }


  void WriterFLAT::writeProfile2D(std::ostream& os, const Profile2D& h) {
    Scatter3D tmp = mkScatter(h);
    tmp.setAnnotation("Type", "Profile2D");
    writeScatter3D(os, tmp);
  }




  void WriterFLAT::writeScatter1D(std::ostream& os, const Scatter1D& s) {
    ios_base::fmtflags oldflags = os.flags();
    os << scientific << showpoint << setprecision(_precision);

    os << "# BEGIN HISTOGRAM " << s.path() << "\n";
    _writeAnnotations(os, s);
    os << "# value\t errminus\t errplus\n";
    BOOST_FOREACH (const Point1D& pt, s.points()) {
      os << pt.x() << "\t" << pt.xErrMinus() << "\t" << pt.xErrPlus() << "\n";
    }
    os << "# END HISTOGRAM\n\n";

    os << flush;
    os.flags(oldflags);
  }


  void WriterFLAT::writeScatter2D(std::ostream& os, const Scatter2D& s) {
    ios_base::fmtflags oldflags = os.flags();
    os << scientific << showpoint << setprecision(_precision);

    /// @todo Write as HISTO1D in future
    os << "# BEGIN HISTOGRAM " << s.path() << "\n";
    _writeAnnotations(os, s);
    os << "# xlow\t xhigh\t val\t errminus\t errplus\n";
    BOOST_FOREACH (const Point2D& pt, s.points()) {
      os << pt.x()-pt.xErrMinus() << "\t" << pt.x()+pt.xErrPlus() << "\t";
      os << pt.y() << "\t" << pt.yErrMinus() << "\t" << pt.yErrPlus() << "\n";
    }
    /// @todo Write as HISTO1D in future
    os << "# END HISTOGRAM\n\n";

    os << flush;
    os.flags(oldflags);
  }


  void WriterFLAT::writeScatter3D(std::ostream& os, const Scatter3D& s) { // , bool asHist2D) {
    ios_base::fmtflags oldflags = os.flags();
    os << scientific << showpoint << setprecision(_precision);

    /// @todo Alternative format to allow writing as unbinned scatters?
    /// Need same for Scatter2D if so... I think better to use YODA format for that
    /// @todo Write as HISTO2D in future
    os << "# BEGIN HISTOGRAM " << s.path() << "\n";
    _writeAnnotations(os, s);

    // if (asHist2D) { // Extension of what writeScatter2D does
    os << "# xlow\t xhigh\t ylow\t yhigh\t val\t errminus\t errplus\n";
      BOOST_FOREACH (const Point3D& pt, s.points()) {
        os << pt.x()-pt.xErrMinus() << "\t" << pt.x()+pt.xErrPlus() << "\t";
        os << pt.y()-pt.yErrMinus() << "\t" << pt.y()+pt.yErrPlus() << "\t";
        os << pt.z() << "\t" << pt.zErrMinus() << "\t" << pt.zErrPlus() << "\n";
      }

    // } else { // What writerYODA should do... let's just put this in there (generalised for multiple errs).
    //   os << "# xval\t xerr-\t xerr+\t yval\t yerr-\t yerr+\t zval\t zerr-\t zerr+\n";
    //   BOOST_FOREACH (const Point3D& pt, s.points()) {
    //     os << pt.x() << "\t" << pt.xErrMinus() << "\t" << pt.xErrMinus() << "\t";
    //     os << pt.y() << "\t" << pt.yErrMinus() << "\t" << pt.yErrMinus() << "\t";
    //     os << pt.z() << "\t" << pt.zErrMinus() << "\t" << pt.zErrMinus() << "\n";
    //   }
    // }

    /// @todo Write as HISTO2D in future
    os << "# END HISTOGRAM\n\n";

    os << flush;
    os.flags(oldflags);
  }


}
