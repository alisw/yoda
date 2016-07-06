// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2015 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_AnalysisObject_h
#define YODA_AnalysisObject_h

#include "YODA/Exceptions.h"
#include "YODA/Config/BuildConfig.h"
#include <string>
#include <map>

namespace YODA {


  /// AnalysisObject is the base class for histograms and scatters
  class AnalysisObject {

  public:

    /// Collection type for annotations, as a string-string map.
    typedef std::map<std::string, std::string> Annotations;


    /// @name Creation and destruction
    //@{

    /// Default constructor
    AnalysisObject() { }

    /// Constructor giving a type, a path and an optional title
    AnalysisObject(const std::string& type, const std::string& path, const std::string& title="") {
      setAnnotation("Type", type);
      setPath(path);
      setTitle(title);
    }

    /// Constructor giving a type, a path, another AO to copy annotation from, and an optional title
    AnalysisObject(const std::string& type, const std::string& path,
                   const AnalysisObject& ao, const std::string& title="") {
      BOOST_FOREACH (const std::string& a, ao.annotations())
        setAnnotation(a, ao.annotation(a));
      setAnnotation("Type", type); // might override the copied ones
      setPath(path);
      setTitle(title);
    }

    // /// Default copy constructor
    // AnalysisObject(const AnalysisObject& ao) {
    //   if (ao.path().length() > 0) setPath(ao.path());
    //   if (ao.title().length() > 0) setTitle(ao.title());
    // }

    /// Default destructor
    virtual ~AnalysisObject() { }

    /// Default copy assignment operator
    virtual AnalysisObject& operator = (const AnalysisObject& ao) {
      if (ao.path().length() > 0) setPath(ao.path());
      if (ao.title().length() > 0) setTitle(ao.title());
      return *this;
    }

    /// Make a copy on the heap, via 'new'
    virtual AnalysisObject* newclone() const = 0;

    //@}



    /// @name Modifiers
    //@{

    /// Reset this analysis object
    virtual void reset() = 0;

    //@}



    ///@name Annotations
    //@{

    /// Get all the annotation names
    const std::vector<std::string> annotations() const {
      std::vector<std::string> rtn;
      rtn.reserve(_annotations.size());
      BOOST_FOREACH (const Annotations::value_type& kv, _annotations) rtn.push_back(kv.first);
      return rtn;
    }


    /// Check if an annotation is defined
    bool hasAnnotation(const std::string& name) const {
      return _annotations.find(name) != _annotations.end();
    }


    /// Get an annotation by name (as a string)
    const std::string& annotation(const std::string& name) const {
      Annotations::const_iterator v = _annotations.find(name);
      // If not found... written this way round on purpose
      if (v == _annotations.end()) {
        std::string missing = "YODA::AnalysisObject: No annotation named " + name;
        throw AnnotationError(missing);
      }
      return v->second;
    }


    /// Get an annotation by name (as a string) with a default in case the annotation is not found
    const std::string& annotation(const std::string& name, const std::string& defaultreturn) const {
      Annotations::const_iterator v = _annotations.find(name);
      if (v != _annotations.end()) return v->second;
      return defaultreturn;
    }


    /// @brief Get an annotation by name (copied to another type)
    ///
    /// @note Templated on return type
    template <typename T>
    const T annotation(const std::string& name) const {
      std::string s = annotation(name);
      return boost::lexical_cast<T>(s);
    }


    /// @brief Get an annotation by name (copied to another type) with a default in case the annotation is not found
    ///
    /// @note Templated on return type
    template <typename T>
    const T annotation(const std::string& name, const T& defaultreturn) const {
      try {
        std::string s = annotation(name);
        return boost::lexical_cast<T>(s);
      } catch (const AnnotationError& ae) {
        return defaultreturn;
      }
    }


    /// @brief Add or set an annotation by name
    ///
    /// @note Templated on arg type, but stored as a string.
    template <typename T>
    void setAnnotation(const std::string& name, const T& value) {
      _annotations[name] = boost::lexical_cast<std::string>(value);
      /// @todo Specialise for float, double, etc. with this safer recipe from the Boost docs:
      // std::stringstream ss;
      // ss << setprecison(std::numeric_limits<double>::max_digits10) << scientific << output_value;
    }


    /// Set all annotations at once
    void setAnnotations(const Annotations& anns) {
      _annotations = anns;
    }


    /// @brief Add or set an annotation by name
    ///
    /// Note: Templated on arg type, but stored as a string. This is just a synonym for setAnnotation.
    template <typename T>
    void addAnnotation(const std::string& name, const T& value) {
      setAnnotation(name, value);
    }


    /// Delete an annotation by name
    void rmAnnotation(const std::string& name) {
      _annotations.erase(name);
    }


    /// Delete an annotation by name
    void clearAnnotations() {
      _annotations.clear();
    }

    //@}


    /// @name Standard annotations
    //@{

    /// Get the AO title.
    /// Returns a null string if undefined, rather than throwing an exception cf. the Title annotation.
    const std::string title() const {
      try {
        return annotation("Title");
      } catch (AnnotationError& ae) {
        return "";
      }
    }

    /// Set the AO title
    void setTitle(const std::string& title) {
      setAnnotation("Title", title);
    }

    /// Get the AO path.
    /// Returns a null string if undefined, rather than throwing an exception cf. the Title annotation.
    const std::string path() const {
      try {
        return annotation("Path");
      } catch (AnnotationError& ae) {
        return "";
      }
    }

    /// Set the AO path
    void setPath(const std::string& path) {
      if (path.length() > 0 && path.find("/") != 0) {
        throw AnnotationError("Histo paths must start with a slash (/) character.");
      }
      setAnnotation("Path", path);
    }


    /// Get the AO name -- the last part of the path.
    /// Returns a null string if path is undefined
    const std::string name() const {
      const std::string p = path();
      const size_t lastslash = p.rfind("/");
      if (lastslash == std::string::npos) return p;
      return p.substr(lastslash+1);
    }

    //@}


  public:

    /// @name Persistency hooks
    //@{

    /// Get name of the analysis object type, for persistency
    virtual std::string type() const {
      return annotation("Type");
    }

    //@}


  private:

    /// The annotations indexed by name
    std::map<std::string,std::string> _annotations;

  };


}

#endif // YODA_AnalysisObject_h
