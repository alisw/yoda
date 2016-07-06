"""Readers and writers

The basic idea here is to provide Python IO semantics by using Python to do
the IO. Otherwise we get C++ IO semantics in Python. It also means we can use
dummy files, e.g. anything with read/write attributes. Generally a much better
idea than just 'give this a filename', and well worth the inefficiencies and
potential memory limits.
"""

import sys

## Check if a string matches any of the given patterns, and that it doesn't match any unpatterns (for path filtering)
def _pattern_check(name, patterns, unpatterns):
    import re
    if patterns:
        if not hasattr(patterns, "__iter__"):
            patterns = [patterns]
        ## Compile on the fly: works because compile(compiled_re) -> compiled_re
        if not any(re.compile(patt).match(name) for patt in patterns):
            return False
    if unpatterns:
        if not hasattr(unpatterns, "__iter__"):
            unpatterns = [unpatterns]
        ## Compile on the fly: works because compile(compiled_re) -> compiled_re
        if any(re.compile(patt).match(name) for patt in unpatterns):
            return False
    return True

## Make a Python list of analysis objects from a C++ vector of them
cdef list _aobjects_to_list(vector[c.AnalysisObject*]* aobjects, patterns, unpatterns):
    cdef list out = []
    cdef c.AnalysisObject* ao
    cdef size_t i
    for i in xrange(aobjects.size()):
        ao = deref(aobjects)[i]
        ## NOTE: automatic type conversion by passing the type() as a key to globals()
        newao = cutil.new_owned_cls(globals()[ao.type()], ao)
        if _pattern_check(newao.path, patterns, unpatterns):
            out.append(newao)
    return out

## Make a Python dict of analysis objects from a C++ vector of them
cdef dict _aobjects_to_dict(vector[c.AnalysisObject*]* aobjects, patterns, unpatterns):
    cdef dict out = {}
    cdef c.AnalysisObject* ao
    cdef size_t i
    for i in xrange(aobjects.size()):
        ao = deref(aobjects)[i]
        ## NOTE: automatic type conversion by passing the type() as a key to globals()
        newao = cutil.new_owned_cls( globals()[ao.type()], ao)
        if _pattern_check(newao.path, patterns, unpatterns):
            out[newao.path] = newao
    return out

## Set a istringstream's string from a C/Python string
cdef void _make_iss(c.istringstream &iss, char* s):
    iss.str(string(s))

## Read a file's contents as a returned string
## The file argument can either be a file object, filename, or special "-" reference to stdin
def _str_from_file(file_or_filename):
    if hasattr(file_or_filename, 'read'):
        s = file_or_filename.read()
    elif file_or_filename == "-":
        s = sys.stdin.read()
    else:
        f = open(file_or_filename)
        s = f.read()
        f.close()
    return s

## Write a string to a file
## The file argument can either be a file object, filename, or special "-" reference to stdout
def _str_to_file(s, file_or_filename):
    if hasattr(file_or_filename, 'write'):
        file_or_filename.write(s)
    elif file_or_filename == "-":
        sys.stdout.write(s)
    else:
        f = open(file_or_filename, "w")
        s = f.write(s)
        f.close()



##
## Readers
##

def read(filename, asdict=True, patterns=None, unpatterns=None):
    """
    Read data objects from the provided filename, auto-determining the format
    from the file extension.

    The loaded data objects can be filtered on their path strings, using the
    optional patterns and unpatterns arguments. These can be strings, compiled
    regex objects with a 'match' method, or any iterable of those types. If
    given, only analyses with paths which match at least one pattern, and do not
    match any unpatterns, will be returned.

    Returns a dict or list of analysis objects depending on the asdict argument.
    """
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects
    f = open(filename)
    s = f.read()
    f.close()
    _make_iss(iss, s)
    c.Reader_create(filename).read(iss, aobjects)
    return _aobjects_to_dict(&aobjects, patterns, unpatterns) if asdict else _aobjects_to_list(&aobjects, patterns, unpatterns)


def readYODA(file_or_filename, asdict=True, patterns=None, unpatterns=None):
    """
    Read data objects from the provided YODA-format file.

    The loaded data objects can be filtered on their path strings, using the
    optional patterns and unpatterns arguments. These can be strings, compiled
    regex objects with a 'match' method, or any iterable of those types. If
    given, only analyses with paths which match at least one pattern, and do not
    match any unpatterns, will be returned.

    Returns a dict or list of analysis objects depending on the asdict argument.
    """
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects
    s = _str_from_file(file_or_filename)
    _make_iss(iss, s)
    c.ReaderYODA_create().read(iss, aobjects)
    # TODO: Add optional filter pattern in conversion to Python iterable (also for all other read functions)
    return _aobjects_to_dict(&aobjects, patterns, unpatterns) if asdict else _aobjects_to_list(&aobjects, patterns, unpatterns)


def readFLAT(file_or_filename, asdict=True, patterns=None, unpatterns=None):
    """
    Read data objects from the provided FLAT-format file.

    The loaded data objects can be filtered on their path strings, using the
    optional patterns and unpatterns arguments. These can be strings, compiled
    regex objects with a 'match' method, or any iterable of those types. If
    given, only analyses with paths which match at least one pattern, and do not
    match any unpatterns, will be returned.

    Returns a dict or list of analysis objects depending on the asdict argument.
    """
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects
    s = _str_from_file(file_or_filename)
    _make_iss(iss, s)
    c.ReaderFLAT_create().read(iss, aobjects)
    return _aobjects_to_dict(&aobjects, patterns, unpatterns) if asdict else _aobjects_to_list(&aobjects, patterns, unpatterns)


def readAIDA(file_or_filename, asdict=True, patterns=None, unpatterns=None):
    """
    Read data objects from the provided AIDA-format file.

    The loaded data objects can be filtered on their path strings, using the
    optional patterns and unpatterns arguments. These can be strings, compiled
    regex objects with a 'match' method, or any iterable of those types. If
    given, only analyses with paths which match at least one pattern, and do not
    match any unpatterns, will be returned.

    Returns a dict or list of analysis objects depending on the asdict argument.

    DEPRECATED: AIDA is a dead format. At some point we will stop supporting it.
    """
    cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects
    s = _str_from_file(file_or_filename)
    _make_iss(iss, s)
    c.ReaderAIDA_create().read(iss, aobjects)
    return _aobjects_to_dict(&aobjects, patterns, unpatterns) if asdict else _aobjects_to_list(&aobjects, patterns, unpatterns)


##
## Writers
##

def write(ana_objs, filename):
    """
    Write data objects to the provided filename,
    auto-determining the format from the file extension.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    aolist = ana_objs.values() if hasattr(ana_objs, "values") else ana_objs if hasattr(ana_objs, "__iter__") else [ana_objs]
    for a in aolist:
        vec.push_back(a._AnalysisObject())

    c.Writer_create(filename).write(oss, vec)
    _str_to_file(oss.str().c_str(), filename)


def writeYODA(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in YODA format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    aolist = ana_objs.values() if hasattr(ana_objs, "values") else ana_objs if hasattr(ana_objs, "__iter__") else [ana_objs]
    for a in aolist:
        vec.push_back(a._AnalysisObject())

    c.WriterYODA_create().write(oss, vec)
    _str_to_file(oss.str().c_str(), file_or_filename)


def writeFLAT(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in FLAT format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    aolist = ana_objs.values() if hasattr(ana_objs, "values") else ana_objs if hasattr(ana_objs, "__iter__") else [ana_objs]
    for a in aolist:
        vec.push_back(a._AnalysisObject())

    c.WriterFLAT_create().write(oss, vec)
    _str_to_file(oss.str().c_str(), file_or_filename)


def writeAIDA(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in AIDA format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a

    aolist = ana_objs.values() if hasattr(ana_objs, "values") else ana_objs if hasattr(ana_objs, "__iter__") else [ana_objs]
    for a in aolist:
        vec.push_back(a._AnalysisObject())

    c.WriterAIDA_create().write(oss, vec)
    _str_to_file(oss.str().c_str(), file_or_filename)
