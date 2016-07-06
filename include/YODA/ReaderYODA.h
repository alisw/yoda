// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_READERYODA_H
#define YODA_READERYODA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Reader.h"
#include <YODA/Counter.h>
#include <YODA/Histo1D.h>
#include <YODA/Histo2D.h>
#include <YODA/Profile1D.h>
#include <YODA/Profile2D.h>
#include <YODA/Scatter1D.h>
#include <YODA/Scatter2D.h>
//#include <YODA/Scatter3D.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace YODA {

  using namespace boost::spirit;
  using namespace boost::phoenix;


  /// Persistency reader from YODA flat text data format.
  class ReaderYODA : public Reader {
  public:

    /// Singleton creation function
    static Reader& create() {
      static ReaderYODA _instance;
      return _instance;
    }

    void read(std::istream& stream, std::vector<AnalysisObject*>& aos) {
      _readDoc(stream, aos);
    }


    // Hide from Doxygen until endcond
    /// @cond


  protected:

    void _readDoc(std::istream& stream, std::vector<AnalysisObject*>& aos);

  private:

    void cleanup() {
      _counter.clear();

      _histo1d.bins.clear();
      _histo1d.dbn_tot.reset();
      _histo1d.dbn_uflow.reset();
      _histo1d.dbn_oflow.reset();

      _histo2d.bins.clear();
      _histo2d.dbn_tot.reset();
      /// @todo For now just create 8 fake entries: needs to be greatly generalised for final form
      _histo2d.dbns_oflow.resize(8);

      _profile1d.bins.clear();
      _profile1d.dbn_tot.reset();
      _profile1d.dbn_uflow.reset();
      _profile1d.dbn_oflow.reset();

      _profile2d.bins.clear();
      _profile2d.dbn_tot.reset();
      /// @todo For now just create 8 fake entries: needs to be greatly generalised for final form
      _profile2d.dbns_oflow.resize(8);

      _scatter1d.points.clear();

      _scatter2d.points.clear();

      _scatter3d.points.clear();

      _annotations.clear();
    }

  public:

    /// Private constructor, since it's a singleton.
    /// @todo This is definitely not private!
    ReaderYODA() { }


    // Here comes everything we need for the parser

    /// The data for the Counter
    struct counter {
      double sumW;
      double sumW2;
      unsigned long numFills;
      void clear() { sumW = 0; sumW2 = 0; numFills = 0; }
    };
    static counter _counter;


    /// The data for the Dbn1D in Histo1D
    struct histo1ddbn {
      double sumW;
      double sumW2;
      double sumWX;
      double sumWX2;
      unsigned long numFills;
    };

    /// The data for the Dbn2D in Profile1D (except sumWXY, since unused by profiles)
    struct profile1ddbn {
      double sumW;
      double sumW2;
      double sumWX;
      double sumWX2;
      double sumWY;
      double sumWY2;
      // double sumWXY;
      unsigned long numFills;
    };

    /// The data for the Dbn2D in Histo2D
    struct histo2ddbn {
      double sumW;
      double sumW2;
      double sumWX;
      double sumWX2;
      double sumWY;
      double sumWY2;
      double sumWXY;
      unsigned long numFills;
    };

    /// The data for Dbn3D in Profile2D (except sumWXZ and sumYZ, since unused by profiles)
    struct profile2ddbn {
      double sumW;
      double sumW2;
      double sumWX;
      double sumWX2;
      double sumWY;
      double sumWY2;
      double sumWZ;
      double sumWZ2;
      double sumWXY;
      // double sumWXZ;
      // double sumWYZ;
      unsigned long numFills;
    };


    /// A Histo1D bin
    struct histo1dbin {
      double xlow;
      double xhigh;
      histo1ddbn dbn;
    };

    /// A Histo2D bin
    struct histo2dbin {
      double xlow;
      double xhigh;
      double ylow;
      double yhigh;
      histo2ddbn dbn;
    };

    /// A Profile1D bin
    struct profile1dbin {
      double xlow;
      double xhigh;
      profile1ddbn dbn;
    };

    /// A Profile2D bin
    struct profile2dbin {
      double xlow;
      double xhigh;
      double ylow;
      double yhigh;
      profile2ddbn dbn;
    };

    /// The data for Point1D
    struct scatter1dpoint {
      double x;
      double exminus;
      double explus;
    };

    /// The data for Point2D
    struct scatter2dpoint {
      double x;
      double exminus;
      double explus;
      double y;
      double eyminus;
      double eyplus;
    };

    /// The data for Point3D
    struct scatter3dpoint {
      double x;
      double exminus;
      double explus;
      double y;
      double eyminus;
      double eyplus;
      double z;
      double ezminus;
      double ezplus;
    };


    /// Structs for the key-value pair parser (annotations)
    struct keyval {
      std::string key;
      std::string val;
    };
    /// Annotations (common for all data types)
    static std::map<std::string, std::string> _annotations;


    /// All information for creating a Histo1D
    struct histo1d {
      std::vector<YODA::HistoBin1D> bins;
      YODA::Dbn1D dbn_tot;
      YODA::Dbn1D dbn_uflow;
      YODA::Dbn1D dbn_oflow;
    };
    static histo1d _histo1d;

    /// All information for creating a Histo2D
    struct histo2d {
      std::vector<YODA::HistoBin2D> bins;
      YODA::Dbn2D dbn_tot;
      std::vector< std::vector<YODA::Dbn2D> > dbns_oflow;
    };
    static histo2d _histo2d;

    /// All information for creating a Profile1D
    struct profile1d {
      std::vector<YODA::ProfileBin1D> bins;
      YODA::Dbn2D dbn_tot;
      YODA::Dbn2D dbn_uflow;
      YODA::Dbn2D dbn_oflow;
    };
    static profile1d _profile1d;

    /// All information for creating a Profile2D
    struct profile2d {
      std::vector<YODA::ProfileBin2D> bins;
      YODA::Dbn3D dbn_tot;
      std::vector< std::vector<YODA::Dbn3D> > dbns_oflow;
    };
    static profile2d _profile2d;

    /// All information for creating a Scatter1D
    struct scatter1d {
      std::vector<YODA::Point1D> points;
    };
    static scatter1d _scatter1d;

    /// All information for creating a Scatter2D
    struct scatter2d {
      std::vector<YODA::Point2D> points;
    };
    static scatter2d _scatter2d;

    /// All information for creating a Scatter3D
    struct scatter3d {
      std::vector<YODA::Point3D> points;
    };
    static scatter3d _scatter3d;


    /// Functions to call from the parser
    /// Filling the dbn_tot
    struct filltotaldbn {
      void operator()(const histo1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _histo1d.dbn_tot = YODA::Dbn1D(dbn.numFills, dbn.sumW, dbn.sumW2,
                                       dbn.sumWX, dbn.sumWX2);
      }
      void operator()(const profile1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _profile1d.dbn_tot = YODA::Dbn2D(dbn.numFills, dbn.sumW, dbn.sumW2,
                                         dbn.sumWX, dbn.sumWX2, dbn.sumWY, dbn.sumWY2, 0.0);
      }
      void operator()(const histo2ddbn dbn, qi::unused_type, qi::unused_type) const {
        _histo2d.dbn_tot = YODA::Dbn2D(dbn.numFills, dbn.sumW, dbn.sumW2,
                                       dbn.sumWX, dbn.sumWX2, dbn.sumWY, dbn.sumWY2, dbn.sumWXY);
      }
      void operator()(const profile2ddbn dbn, qi::unused_type, qi::unused_type) const {
        _profile2d.dbn_tot = YODA::Dbn3D(dbn.numFills, dbn.sumW, dbn.sumW2,
                                         dbn.sumWX, dbn.sumWX2, dbn.sumWY, dbn.sumWY2, dbn.sumWZ, dbn.sumWZ2,
                                         dbn.sumWXY, 0.0, 0.0);
      }
    };


    /// Filling the underflow
    struct filluflowdbn {
      void operator()(const histo1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _histo1d.dbn_uflow = YODA::Dbn1D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2);
      }
      void operator()(const profile1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _profile1d.dbn_uflow = YODA::Dbn2D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2, dbn.sumWY, dbn.sumWY2, 0.0);
      }
    };


    /// Filling the overflow
    struct filloflowdbn {
      void operator()(const histo1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _histo1d.dbn_oflow = YODA::Dbn1D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2);
      }
      void operator()(const profile1ddbn dbn, qi::unused_type, qi::unused_type) const {
        _profile1d.dbn_oflow = YODA::Dbn2D(dbn.numFills, dbn.sumW, dbn.sumW2, dbn.sumWX, dbn.sumWX2, dbn.sumWY, dbn.sumWY2, 0.0);
      }
    };


    /// @todo Add filling 2D outflows


    /// Filling a bin
    struct fillbin {
      void operator()(const histo1dbin b, qi::unused_type, qi::unused_type) const {
        YODA::HistoBin1D bin(std::make_pair(b.xlow, b.xhigh),
                             YODA::Dbn1D(b.dbn.numFills, b.dbn.sumW, b.dbn.sumW2, b.dbn.sumWX, b.dbn.sumWX2));
        _histo1d.bins.push_back(bin);
      }
      void operator()(const profile1dbin b, qi::unused_type, qi::unused_type) const {
        YODA::ProfileBin1D bin(std::make_pair(b.xlow, b.xhigh),
                               YODA::Dbn2D(b.dbn.numFills, b.dbn.sumW, b.dbn.sumW2,
                                           b.dbn.sumWX, b.dbn.sumWX2, b.dbn.sumWY, b.dbn.sumWY2, 0.0));
        _profile1d.bins.push_back(bin);
      }
      void operator()(const histo2dbin b, qi::unused_type, qi::unused_type) const {
        YODA::HistoBin2D bin(std::make_pair(b.xlow, b.xhigh), std::make_pair(b.ylow, b.yhigh),
                             YODA::Dbn2D(b.dbn.numFills, b.dbn.sumW, b.dbn.sumW2,
                                         b.dbn.sumWX, b.dbn.sumWX2, b.dbn.sumWY, b.dbn.sumWY2, b.dbn.sumWXY));
        _histo2d.bins.push_back(bin);
      }
      void operator()(const profile2dbin b, qi::unused_type, qi::unused_type) const {
        YODA::ProfileBin2D bin(std::make_pair(b.xlow, b.xhigh), std::make_pair(b.ylow, b.yhigh),
                               YODA::Dbn3D(b.dbn.numFills, b.dbn.sumW, b.dbn.sumW2,
                                           b.dbn.sumWX, b.dbn.sumWX2, b.dbn.sumWY, b.dbn.sumWY2, b.dbn.sumWZ, b.dbn.sumWZ2,
                                           b.dbn.sumWXY, 0.0, 0.0));
        _profile2d.bins.push_back(bin);
      }
    };


    /// Filling a point
    struct fillpoint {
      void operator()(const scatter1dpoint p, qi::unused_type, qi::unused_type) const {
        YODA::Point1D point(p.x, p.exminus, p.explus);
        _scatter1d.points.push_back(point);
      }
      void operator()(const scatter2dpoint p, qi::unused_type, qi::unused_type) const {
        YODA::Point2D point(p.x, p.y, p.exminus, p.explus, p.eyminus, p.eyplus);
        _scatter2d.points.push_back(point);
      }
      void operator()(const scatter3dpoint p, qi::unused_type, qi::unused_type) const {
        YODA::Point3D point(p.x, p.y, p.z, p.exminus, p.explus, p.eyminus, p.eyplus, p.ezminus, p.ezplus);
        _scatter3d.points.push_back(point);
      }
    };


    /// Filling the annotations map
    struct fillkeyval {
      void operator()(const keyval m, qi::unused_type, qi::unused_type) const {
        _annotations[m.key] = m.val;
      }
    };


    /// A helper grammar for determining in which context we are.
    /// bgroup and egroup are dynamic parsers. We add and remove
    /// the "# BEGIN SOMETHING" and "# END SOMETHING" strings
    /// dynamically.
    static qi::symbols<char, int> bgroup;
    static qi::symbols<char, int> egroup;
    template <typename Iterator>
    struct group_grammar : qi::grammar<Iterator, int()>
    {
      group_grammar() : group_grammar::base_type(start) {
        start = begin | end;
        begin = qi::eps [_val = 0]  >>
                qi::lit("# BEGIN ") >>
                bgroup  [_val += _1];
        end   = qi::eps [_val = 0]  >>
                qi::lit("# END ")   >>
                egroup  [_val += _1];
      }
      qi::rule<Iterator, int()> start, begin, end;
    };


    /// The actual grammar for parsing the lines of a flat data file.
    template <typename Iterator, typename Skipper>
    struct yoda_grammar : qi::grammar<Iterator, Skipper> {

      yoda_grammar() : yoda_grammar::base_type(line) {

        /// @note A line can be anything. Note that we need to specify the long lines first, because the first match wins.
        /// @todo Refactor so that only the appropriate content lines can match in each type-block: anyway _needed_ to allow Scatter3D

        // In brackets we specify the functions that are called if the rule matches.
        line = \
          Profile2Dbin[fillbin()]         | // w w2 x1 x2 y1 y2 wx wx2 wy wy2 wz wz2 wxy n = 14 nums
          Profile2Dtotal[filltotaldbn()]  | // "
          //Profile2Doflow[filloflowdbn()]| // "
          Histo2Dbin[fillbin()]           | // w w2 x1 x2 y1 y2 wx wx2 wy wy2 wxy n = 12 nums
          Histo2Dtotal[filltotaldbn()]    | // "
          //Histo2Doflow[filloflowdbn()]  | // "
          Profile1Dbin[fillbin()]         | // w w2 x1 x2 wx wx2 wy wy2 n = 9 nums
          Profile1Dtotal[filltotaldbn()]  | // "
          Profile1Duflow[filluflowdbn()]  | // "
          Profile1Doflow[filloflowdbn()]  | // "
          Histo1Dbin[fillbin()]           | // w w2 x1 x2 wx wx2 n = 7 nums
          Histo1Dtotal[filltotaldbn()]    | // "
          Histo1Duflow[filluflowdbn()]    | // "
          Histo1Doflow[filloflowdbn()]    | // "
          /// @todo Note clash of Scatter3Dpoint with Profile1Dbin: need "scoped" content line parsing
          //Scatter3Dpoint[fillpoint()]   | // x y z ex- ex+ ey- ey+ ez- ez+ = 9 (+ arbitrarily more sets of errors as 6 doubles... and names?)
          Scatter2Dpoint[fillpoint()]     | // x y ex- ex+ ey- ey+ = 6 (+ arbitrarily more sets of errors as 4 doubles... and names?)
          Scatter1Dpoint[fillpoint()]     | // x ex- ex+ = 3 (+ arbitrarily more sets of errors as 2 doubles... and names?)
          keyvaluepair[fillkeyval()]      |
          comment;

        /// Match the strings "Underflow", "Overflow" and "Total"
        underflow = qi::lit("Underflow");
        overflow  = qi::lit("Overflow");
        total     = qi::lit("Total");

        /// Histo1D
        /// Regular bins, total statistics, underflow or overflow.
        Histo1Dbin   %= double_   >> double_   >> Histo1Ddbn;
        Histo1Dtotal %= total     >> total     >> Histo1Ddbn;
        Histo1Duflow %= underflow >> underflow >> Histo1Ddbn;
        Histo1Doflow %= overflow  >> overflow  >> Histo1Ddbn;
        Histo1Ddbn = (double_ >> double_) >> (double_ >> double_) >> ulong_;

        // Histo2D
        /// Regular bins, total statistics, outflows.
        Histo2Dbin   %= double_   >> double_   >> double_    >> double_   >> Histo2Ddbn;
        Histo2Dtotal %= total     >> total     >> Histo2Ddbn;
        //Histo2Doflow %= overflow  >> overflow  >> Histo2Ddbn;
        Histo2Ddbn = (double_ >> double_) >> (double_ >> double_) >> (double_ >> double_) >> double_ >> ulong_;

        // Profile1D
        /// Regular bins, total statistics, underflow or overflow.
        Profile1Dbin   %= double_   >> double_   >> Profile1Ddbn;
        Profile1Dtotal %= total     >> total     >> Profile1Ddbn;
        Profile1Duflow %= underflow >> underflow >> Profile1Ddbn;
        Profile1Doflow %= overflow  >> overflow  >> Profile1Ddbn;
        Profile1Ddbn = (double_ >> double_) >> (double_ >> double_) >> (double_ >> double_) >> ulong_;

        // Profile2D
        /// Regular bins, total statistics, outflows.
        Profile2Dbin   %= double_   >> double_   >> double_    >> double_   >> Profile2Ddbn;
        Profile2Dtotal %= total     >> total     >> Profile2Ddbn;
        //Profile2Doflow %= overflow  >> overflow  >> Profile2Ddbn;
        Profile2Ddbn = (double_ >> double_) >> (double_ >> double_) >> (double_ >> double_) >> (double_ >> double_) >> double_ >> ulong_;

        // Scatter1D
        Scatter1Dpoint %= double_ >> double_ >> double_;

        // Scatter2D
        Scatter2Dpoint %= double_ >> double_ >> double_ >> double_ >> double_ >> double_;

        // Scatter3D
        // Scatter2Dpoint %= double_ >> double_ >> double_ >> double_ >> double_ >> double_ >> double_ >> double_ >> double_;


        /// Annotations.
        /// The key is anything up to the first equal sign, but
        /// keys can't start with a number or a minus sign. The
        /// value is anything after the equal sign.
        key = !qi::char_("0-9-") >> *~qi::char_("=");
        value = *~qi::char_("\n"); //< DOS/Unix line return safe?
        keyvaluepair %= key >> "=" >> value;

        /// Lines starting with a "#" are comments.
        comment = qi::lit("#") >> *~qi::char_("\n"); //< DOS/Unix line return safe?
      }

      /// In the rules, the first template argument is the Iterator for the string,
      /// the second one is the return type, and the last one is a "Skipper" for
      /// ignoring whitespace. Note that the key/value pair doesn't ignore whitespace.
      /// Most of the return values match our structs (like keyval, histo1dbin, etc.).
      /// Those are used to directly fill the corresponding data structs.
      qi::rule<Iterator, Skipper> line, total, underflow, overflow, comment;
      qi::rule<Iterator, std::string()> key, value;
      qi::rule<Iterator, keyval(), Skipper> keyvaluepair;

      qi::rule<Iterator, counter(), Skipper> Counter;

      qi::rule<Iterator, histo1dbin(), Skipper> Histo1Dbin;
      qi::rule<Iterator, histo1ddbn(), Skipper> Histo1Ddbn, Histo1Dtotal, Histo1Duflow, Histo1Doflow;

      qi::rule<Iterator, histo2dbin(), Skipper> Histo2Dbin;
      qi::rule<Iterator, histo2ddbn(), Skipper> Histo2Ddbn, Histo2Dtotal; //, Histo2Doflow;

      qi::rule<Iterator, profile1dbin(), Skipper> Profile1Dbin;
      qi::rule<Iterator, profile1ddbn(), Skipper> Profile1Ddbn, Profile1Dtotal, Profile1Duflow, Profile1Doflow;

      qi::rule<Iterator, profile2dbin(), Skipper> Profile2Dbin;
      qi::rule<Iterator, profile2ddbn(), Skipper> Profile2Ddbn, Profile2Dtotal; //, Profile2Doflow;

      qi::rule<Iterator, scatter1dpoint(), Skipper> Scatter1Dpoint;
      qi::rule<Iterator, scatter2dpoint(), Skipper> Scatter2Dpoint;
      //qi::rule<Iterator, scatter3dpoint(), Skipper> Scatter3Dpoint;

    };


    /// @endcond

  };

} // end of YODA namespace



// Now we need to make boost aware of the structs we want to
// fill directly from the parser. Boost wants this magic in
// the global scope, that's why we have it outside the namespace.

/// @cond PRIVATE

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::counter,
  (double, sumW)
  (double, sumW2)
  (unsigned long, numFills)
)


BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::histo1ddbn,
  (double, sumW)
  (double, sumW2)
  (double, sumWX)
  (double, sumWX2)
  (unsigned long, numFills)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::histo1dbin,
  (double, xlow)
  (double, xhigh)
  (YODA::ReaderYODA::histo1ddbn, dbn)
)


BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::histo2ddbn,
  (double, sumW)
  (double, sumW2)
  (double, sumWX)
  (double, sumWX2)
  (double, sumWY)
  (double, sumWY2)
  (double, sumWXY)
  (unsigned long, numFills)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::histo2dbin,
  (double, xlow)
  (double, xhigh)
  (double, ylow)
  (double, yhigh)
  (YODA::ReaderYODA::histo2ddbn, dbn)
)


BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::profile1ddbn,
  (double, sumW)
  (double, sumW2)
  (double, sumWX)
  (double, sumWX2)
  (double, sumWY)
  (double, sumWY2)
  (unsigned long, numFills)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::profile1dbin,
  (double, xlow)
  (double, xhigh)
  (YODA::ReaderYODA::profile1ddbn, dbn)
)


BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::profile2ddbn,
  (double, sumW)
  (double, sumW2)
  (double, sumWX)
  (double, sumWX2)
  (double, sumWY)
  (double, sumWY2)
  (double, sumWZ)
  (double, sumWZ2)
  (double, sumWXY)
  (unsigned long, numFills)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::profile2dbin,
  (double, xlow)
  (double, xhigh)
  (double, ylow)
  (double, yhigh)
  (YODA::ReaderYODA::profile2ddbn, dbn)
)


/// @todo Change ordering to {vals} {errs}
BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::scatter1dpoint,
  (double, x)
  (double, exminus)
  (double, explus)
)

/// @todo Change ordering to {vals} {errs}
BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::scatter2dpoint,
  (double, x)
  (double, exminus)
  (double, explus)
  (double, y)
  (double, eyminus)
  (double, eyplus)
)

/// @todo Change ordering to {vals} {errs}
BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::scatter3dpoint,
  (double, x)
  (double, exminus)
  (double, explus)
  (double, y)
  (double, eyminus)
  (double, eyplus)
  (double, z)
  (double, ezminus)
  (double, ezplus)
)


BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderYODA::keyval,
  (std::string, key)
  (std::string, val)
)

/// @endcond

#endif
