// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2016 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_Writer_h
#define YODA_Writer_h

#include "YODA/AnalysisObject.h"
#include "YODA/Counter.h"
#include "YODA/Histo1D.h"
#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Profile2D.h"
#include "YODA/Scatter1D.h"
#include "YODA/Scatter2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Utils/Traits.h"
#include <type_traits>
#include <fstream>
#include <ostream>
#include <string>

namespace YODA {


  /// Pure virtual base class for various output writers.
  class Writer {
  public:

    /// Virtual destructor
    virtual ~Writer() {}


    /// @name Writing a single analysis object.
    //@{

    /// Write out object @a ao to output stream @a stream.
    void write(std::ostream& stream, const AnalysisObject& ao);

    /// Write out pointer-like object @a ao to output stream @a stream.
    template <typename T>
    typename std::enable_if<DerefableToAO<T>::value>::type //< -> void if valid
    write(std::ostream& stream, const T& ao) { write(stream, *ao); }

    /// Write out object @a ao to file @a filename.
    void write(const std::string& filename, const AnalysisObject& ao);

    /// Write out pointer-like object @a ao to file @a filename.
    template <typename T>
    typename std::enable_if<DerefableToAO<T>::value>::type //< -> void if valid
    write(const std::string& filename, const T& ao) { write(filename, *ao); }

    //@}


    /// @name Writing multiple analysis objects by collection.
    //@{

    /// Write out a collection of objects @a objs to output stream @a stream.
    /// Note: the enable_if call checks whether RANGE is const_iterable, if yes the return
    ///       type is void. If not, this template will not be a candidate in the lookup
    template <typename RANGE>
    typename std::enable_if<CIterable<RANGE>::value>::type
    write(std::ostream& stream, const RANGE& aos) {
      write(stream, std::begin(aos), std::end(aos));
    }

    /// Write out a collection of objects @a objs to file @a filename.
    template <typename RANGE>
    typename std::enable_if<CIterable<RANGE>::value>::type
    write(const std::string& filename, const RANGE& aos) {
      write(filename, std::begin(aos), std::end(aos));
    }

    //@}


    /// @name Writing multiple analysis objects by iterator range.
    //@{

    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to output stream @a stream.
    ///
    /// @todo Add SFINAE trait checking for AOITER = DerefableToAO
    template <typename AOITER>
    void write(std::ostream& stream, const AOITER& begin, const AOITER& end) {
      writeHeader(stream);
      for (AOITER ipao = begin; ipao != end; ++ipao) {
	try {
	  writeBody(stream, *ipao);
	}
	catch (const LowStatsError & ex) {
	  std::cerr << "Not writing out Analysis Object " << (**ipao).title()
	       << " as caught LowStatsError. The exception was\n "
		    << ex.what() << "\n";
	}
      }
      writeFooter(stream);
    }


    /// Write out the objects specified by start iterator @a begin and end
    /// iterator @a end to file @a filename.
    ///
    /// @todo Add SFINAE trait checking for AOITER = DerefableToAO
    template <typename AOITER>
    void write(const std::string& filename,
               const AOITER& begin,
               const AOITER& end) {
      std::ofstream outstream;
      outstream.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
      try{
        outstream.open(filename.c_str());
        write(outstream, begin, end);
        outstream.close();
      } catch(std::ifstream::failure e) {
        throw WriteError("writing to filename " + filename + " failed: " + e.what());
      }
    }

    //@}


    /// Set precision of numerical quantities in this writer's output.
    void setPrecision(int precision) {
      _precision = precision;
    }


  protected:

    /// @name Main writer elements
    //@{

    /// Write any opening boilerplate required by the format to @a stream
    virtual void writeHeader(std::ostream& stream) = 0;

    /// @brief Write the body elements corresponding to AnalysisObject @a ao to @a stream
    virtual void writeBody(std::ostream& stream, const AnalysisObject* ao);

    /// @brief Write the body elements corresponding to AnalysisObject pointer @a ao to @a stream
    virtual void writeBody(std::ostream& stream, const AnalysisObject& ao);

    /// @brief Write the body elements corresponding to AnalysisObject @a ao to @a stream
    /// @note Requires that @a ao is dereferenceable to an AnalysisObject, via the DerefableToAO<T> trait,
    template <typename T>
    typename std::enable_if<DerefableToAO<T>::value>::type //< -> void if valid
    writeBody(std::ostream& stream, const T& ao) { writeBody(stream, *ao); }

    /// Write any closing boilerplate required by the format to @a stream
    virtual void writeFooter(std::ostream& stream) = 0;

    //@}


    /// @name Specific AO type writer implementations, to be implemented in derived classes
    //@{

    virtual void writeCounter(std::ostream& stream, const Counter& c) = 0;
    virtual void writeHisto1D(std::ostream& os, const Histo1D& h) = 0;
    virtual void writeHisto2D(std::ostream& os, const Histo2D& h) = 0;
    virtual void writeProfile1D(std::ostream& os, const Profile1D& p) = 0;
    virtual void writeProfile2D(std::ostream& os, const Profile2D& p) = 0;
    virtual void writeScatter1D(std::ostream& os, const Scatter1D& s) = 0;
    virtual void writeScatter2D(std::ostream& os, const Scatter2D& s) = 0;
    virtual void writeScatter3D(std::ostream& os, const Scatter3D& s) = 0;

    //@}


    /// Output precision
    int _precision;

  };


  /// Factory function to make a writer object by format name or a filename
  Writer& mkWriter(const std::string& format_name);


}

#endif
