// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_READERFLAT_H
#define YODA_READERFLAT_H

#include "YODA/AnalysisObject.h"
#include "YODA/Reader.h"
#include <YODA/Counter.h>
#include <YODA/Scatter1D.h>
#include <YODA/Scatter2D.h>
#include <YODA/Scatter3D.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace YODA {

  using namespace boost::spirit;
  using namespace boost::phoenix;


  /// Persistency reader from YODA flat text data format.
  class ReaderFLAT : public Reader {
  public:

    /// Singleton creation function
    static Reader& create() {
      static ReaderFLAT _instance;
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
      _scatter1d.points.clear();
      _scatter2d.points.clear();
      _scatter3d.points.clear();
      _annotations.clear();
    }

  public:

    /// Private constructor, since it's a singleton.
    /// @todo This is definitely not private!
    ReaderFLAT() { }


    // Here comes everything we need for the parser

    /// Structs for the flat HISTOGRAM parser
    struct histogrampointsymmetric0d {
      double x;
      double ex;
    };

    struct histogrampointasymmetric0d {
      double x;
      double exminus;
      double explus;
    };

    struct histogrampointsymmetric1d {
      double xmin;
      double xmax;
      double y;
      double ey;
    };

    struct histogrampointasymmetric1d {
      double xmin;
      double xmax;
      double y;
      double eyminus;
      double eyplus;
    };

    struct histogrampointsymmetric2d {
      double xmin;
      double xmax;
      double ymin;
      double ymax;
      double z;
      double ez;
    };

    struct histogrampointasymmetric2d {
      double xmin;
      double xmax;
      double ymin;
      double ymax;
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


    /// All information for creating a Scatter1D from a flat HISTOGRAM
    struct scatter1d {
      std::vector<YODA::Point1D> points;
    };
    static scatter1d _scatter1d;


    /// All information for creating a Scatter2D from a flat HISTOGRAM
    struct scatter2d {
      std::vector<YODA::Point2D> points;
    };
    static scatter2d _scatter2d;


    /// All information for creating a Scatter3D from a flat HISTOGRAM
    struct scatter3d {
      std::vector<YODA::Point3D> points;
    };
    static scatter3d _scatter3d;


    /// Functions to call from the parser


    /// Filling a point
    struct fillpoint {
      // 0D
      void operator()(const histogrampointsymmetric0d p, qi::unused_type, qi::unused_type) const {
        //std::cout << "0d symm" << std::endl;
        YODA::Point1D point(p.x, p.ex, p.ex);
        _scatter1d.points.push_back(point);
      }
      void operator()(const histogrampointasymmetric0d p, qi::unused_type, qi::unused_type) const {
        //std::cout << "0d asymm" << std::endl;
        YODA::Point1D point(p.x, p.exminus, p.explus);
        _scatter1d.points.push_back(point);
      }
      // 1D
      void operator()(const histogrampointsymmetric1d p, qi::unused_type, qi::unused_type) const {
        //std::cout << "1d symm" << std::endl;
        double x  = 0.5*(p.xmin+p.xmax);
        double ex = 0.5*(p.xmax-p.xmin);
        YODA::Point2D point(x, p.y, ex, ex, p.ey, p.ey);
        _scatter2d.points.push_back(point);
      }
      void operator()(const histogrampointasymmetric1d p, qi::unused_type, qi::unused_type) const {
        //std::cout << "1d asymm" << std::endl;
        double x  = 0.5*(p.xmin+p.xmax);
        double ex = 0.5*(p.xmax-p.xmin);
        YODA::Point2D point(x, p.y, ex, ex, p.eyminus, p.eyplus);
        _scatter2d.points.push_back(point);
      }
      // 2D
      void operator()(const histogrampointsymmetric2d p, qi::unused_type, qi::unused_type) const {
        //std::cout << "2d symm" << std::endl;
        double x  = 0.5*(p.xmin+p.xmax);
        double ex = 0.5*(p.xmax-p.xmin);
        double y  = 0.5*(p.ymin+p.ymax);
        double ey = 0.5*(p.ymax-p.ymin);
        YODA::Point3D point(x, y, p.z, ex, ex, ey, ey, p.ez, p.ez);
        _scatter3d.points.push_back(point);
      }
      void operator()(const histogrampointasymmetric2d p, qi::unused_type, qi::unused_type) const {
        //std::cout << "2d asymm" << std::endl;
        double x  = 0.5*(p.xmin+p.xmax);
        double ex = 0.5*(p.xmax-p.xmin);
        double y  = 0.5*(p.ymin+p.ymax);
        double ey = 0.5*(p.ymax-p.ymin);
        YODA::Point3D point(x, y, p.z, ex, ex, ey, ey, p.ezminus, p.ezplus);
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
    struct data_grammar : qi::grammar<Iterator, Skipper>
    {

      data_grammar() : data_grammar::base_type(line) {

        /// A line can be anything. Note that we need
        /// to specify the long lines first, because the
        /// first match wins.
        /// In brackets we specify the functions that are
        /// called in case the rule matches.
        line = HistogramPointAsymmetric2D [fillpoint()]  |
               HistogramPointSymmetric2D  [fillpoint()]  |
               HistogramPointAsymmetric1D [fillpoint()]  |
               HistogramPointSymmetric1D  [fillpoint()]  |
               HistogramPointAsymmetric0D [fillpoint()]  |
               HistogramPointSymmetric0D  [fillpoint()]  |
               keyvaluepair[fillkeyval()]                |
               comment;

        // Histogram
        HistogramPointAsymmetric2D %= double_ >> double_ >>   double_ >> double_ >>   double_ >> double_ >> double_;
        HistogramPointSymmetric2D  %= double_ >> double_ >>   double_ >> double_ >>   double_ >> double_;
        HistogramPointAsymmetric1D %= double_ >> double_ >>   double_ >> double_ >> double_;
        HistogramPointSymmetric1D  %= double_ >> double_ >>   double_ >> double_;
        HistogramPointAsymmetric0D %= double_ >> double_ >> double_;
        HistogramPointSymmetric0D  %= double_ >> double_;


        /// Annotations
        /// The key is anything up to the first equal sign, but
        /// keys can't start with a number or a minus sign. The
        /// value is anything after the equal sign.
        key = !qi::char_("0-9-") >> *~qi::char_("=");
        value = *~qi::char_("\n");
        keyvaluepair %= key >> "=" >> value;

        /// Lines starting with a "#" are comments.
        comment = qi::lit("#") >> *~qi::char_("\n");
      }

      /// In the rules, the first template argument is the Iterator for the string,
      /// the second one is the return type, and the last one is a "Skipper" for
      /// ignoring whitespace. Note that the key/value pair doesn't ignore whitespace.
      /// Most of the return values match our structs (like keyval, histo1dbin, etc.).
      /// Those are used to directly fill the corresponding structs.
      qi::rule<Iterator, Skipper> line, comment;
      qi::rule<Iterator, std::string()> key, value;
      qi::rule<Iterator, keyval(), Skipper> keyvaluepair;

      qi::rule<Iterator, histogrampointsymmetric2d(), Skipper> HistogramPointSymmetric2D;
      qi::rule<Iterator, histogrampointasymmetric2d(), Skipper> HistogramPointAsymmetric2D;
      qi::rule<Iterator, histogrampointsymmetric1d(), Skipper> HistogramPointSymmetric1D;
      qi::rule<Iterator, histogrampointasymmetric1d(), Skipper> HistogramPointAsymmetric1D;
      qi::rule<Iterator, histogrampointsymmetric0d(), Skipper> HistogramPointSymmetric0D;
      qi::rule<Iterator, histogrampointasymmetric0d(), Skipper> HistogramPointAsymmetric0D;
    };


    /// @endcond

  };

} // end of YODA namespace



// Now we need to make boost aware of the structs we want to
// fill directly from the parser. Boost wants this magic in
// the global scope, that's why we have it outside the namespace.

/// @cond PRIVATE

/// @todo Can't do Counter without context sensitive parsing, since clashes with symm 0D HISTOGRAM

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderFLAT::histogrampointsymmetric0d,
  (double, x)
  (double, ex)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderFLAT::histogrampointasymmetric0d,
  (double, x)
  (double, exminus)
  (double, explus)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderFLAT::histogrampointsymmetric1d,
  (double, xmin)
  (double, xmax)
  (double, y)
  (double, ey)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderFLAT::histogrampointasymmetric1d,
  (double, xmin)
  (double, xmax)
  (double, y)
  (double, eyminus)
  (double, eyplus)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderFLAT::histogrampointsymmetric2d,
  (double, xmin)
  (double, xmax)
  (double, ymin)
  (double, ymax)
  (double, z)
  (double, ez)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderFLAT::histogrampointasymmetric2d,
  (double, xmin)
  (double, xmax)
  (double, ymin)
  (double, ymax)
  (double, z)
  (double, ezminus)
  (double, ezplus)
)

BOOST_FUSION_ADAPT_STRUCT(
  YODA::ReaderFLAT::keyval,
  (std::string, key)
  (std::string, val)
)

/// @endcond

#endif
