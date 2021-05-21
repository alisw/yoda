# cython: c_string_type=unicode

"""Readers and writers

The basic idea here is to provide Python IO semantics by using Python to do
the IO. Otherwise we get C++ IO semantics in Python. It also means we can use
dummy files, e.g. anything with read/write attributes. Generally a much better
idea than just 'give this a filename', and well worth the inefficiencies and
potential memory limits.
"""

import sys
from libcpp.unordered_map cimport unordered_map
from cython.operator cimport preincrement as inc

## Check if a string matches any of the given patterns, and that it doesn't match any unpatterns (for path filtering)
def _pattern_check(name, patterns, unpatterns):
    import re
    if patterns:
        if not isinstance(patterns, (list,tuple)):
            patterns = [patterns]
        ## Compile on the fly: works because compile(compiled_re) -> compiled_re
        if not any(re.compile(patt).search(name) for patt in patterns):
            return False
    if unpatterns:
        if not isinstance(unpatterns, (list,tuple)):
            unpatterns = [unpatterns]
        ## Compile on the fly: works because compile(compiled_re) -> compiled_re
        if any(re.compile(patt).search(name) for patt in unpatterns):
            return False
    return True

## Make a Python list of analysis objects from a C++ vector of them
cdef list _aobjects_to_list(vector[c.AnalysisObject*]* aobjects, patterns, unpatterns):
    cdef list out = []
    cdef c.AnalysisObject* ao
    cdef size_t i
    for i in range(aobjects.size()):
        ao = deref(aobjects)[i]
        ## NOTE: automatic type conversion by passing the type() as a key to globals()
        newao = cutil.new_owned_cls(globals()[ao.type().decode('utf-8')], ao)
        if _pattern_check(newao.path(), patterns, unpatterns):
            out.append(newao)
    return out

## Make a Python dict of analysis objects from a C++ vector of them
cdef dict _aobjects_to_dict(vector[c.AnalysisObject*]* aobjects, patterns, unpatterns):
    cdef dict out = {}
    # from collections import OrderedDict
    # OrderedDict out = OrderedDict()
    cdef c.AnalysisObject* ao
    cdef size_t i
    for i in range(aobjects.size()):
        ao = deref(aobjects)[i]
        ## NOTE: automatic type conversion by passing the type() as a key to globals()
        newao = cutil.new_owned_cls( globals()[ao.type().decode('utf-8')], ao)
        if _pattern_check(newao.path(), patterns, unpatterns):
            out[newao.path()] = newao
    return out

## Make a Python dict of dicts for file index from a C++ unordered_map of unordered_maps
cdef dict _idxMap_to_dict(unordered_map[string, unordered_map[string, int]]* idxMap):
    cdef dict out = {}
    cdef unordered_map[string, unordered_map[string, int]].iterator it = idxMap.begin()
    cdef dict innerDict = {}
    cdef unordered_map[string, int].iterator it_inner

    while(it != idxMap.end()):
        objType = deref(it).first
        nestedMap = deref(it).second
        innerDict = {}
        it_inner = nestedMap.begin()

        while(it_inner != nestedMap.end()):
            path = deref(it_inner).first
            binNum = deref(it_inner).second
            innerDict[path] = binNum
            inc(it_inner)

        out[objType] = innerDict
        inc(it)

    return out

## Set a istringstream's string from a C/Python string
cdef void _make_iss(c.istringstream &iss, string s):
    iss.str(s)

## Read a file's contents as a returned string
## The file argument can either be a file object, filename, or special "-" reference to stdin
def _str_from_file(file_or_filename):
    if hasattr(file_or_filename, 'read'):
        s = file_or_filename.read()
    elif file_or_filename == "-":
        s = sys.stdin.read()
    else:
        with open(file_or_filename, "r") as f:
            s = f.read()
    return s

## Write a string to a file
## The file argument can either be a file object, filename, or special "-" reference to stdout
def _str_to_file(s, file_or_filename):
    s = s.decode('utf-8')
    if hasattr(file_or_filename, 'write'):
        file_or_filename.write(s)
    elif file_or_filename == "-":
        sys.stdout.write(s)
    else:
        with open(file_or_filename, "w") as f:
            f.write(s)


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
    # cdef c.istringstream iss
    # cdef vector[c.AnalysisObject*] aobjects
    # with open(filename, "r") as f:
    #     s = f.read()
    # _make_iss(iss, s.encode('utf-8'))
    # c.Reader_create(filename.encode('utf-8')).read(iss, aobjects)
    # return _aobjects_to_dict(&aobjects, patterns, unpatterns) if asdict \
    #     else _aobjects_to_list(&aobjects, patterns, unpatterns)
    #
    cdef vector[c.AnalysisObject*] aobjects
    c.IO_read_from_file(filename.encode('utf-8'), aobjects)

    if asdict:
        d = _aobjects_to_dict(&aobjects, patterns, unpatterns)
        try:
            import collections
            return collections.OrderedDict(sorted(d.items()))
        except:
            return d
    else:
        return _aobjects_to_list(&aobjects, patterns, unpatterns)


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
    _make_iss(iss, s.encode('utf-8'))
    c.ReaderYODA_create().read(iss, aobjects)
    # if type(file_or_filename) is str:
    #     c.ReaderYODA_create().read_from_file(file_or_filename.encode('utf-8'), aobjects)
    # else:
    #     c.ReaderYODA_create().read(file_or_filename, aobjects)
    return _aobjects_to_dict(&aobjects, patterns, unpatterns) if asdict \
        else _aobjects_to_list(&aobjects, patterns, unpatterns)


def readFLAT(filename, asdict=True, patterns=None, unpatterns=None):
    """
    Read data objects from the provided FLAT-format file.

    The loaded data objects can be filtered on their path strings, using the
    optional patterns and unpatterns arguments. These can be strings, compiled
    regex objects with a 'match' method, or any iterable of those types. If
    given, only analyses with paths which match at least one pattern, and do not
    match any unpatterns, will be returned.

    Returns a dict or list of analysis objects depending on the asdict argument.
    """
    # cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects
    # s = _str_from_file(file_or_filename)
    # _make_iss(iss, s.encode('utf-8'))
    # c.ReaderFLAT_create().read(iss, aobjects)
    c.ReaderFLAT_create().read_from_file(filename.encode('utf-8'), aobjects)
    return _aobjects_to_dict(&aobjects, patterns, unpatterns) if asdict \
        else _aobjects_to_list(&aobjects, patterns, unpatterns)


def readAIDA(filename, asdict=True, patterns=None, unpatterns=None):
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
    # cdef c.istringstream iss
    cdef vector[c.AnalysisObject*] aobjects
    # s = _str_from_file(file_or_filename)
    # _make_iss(iss, s.encode('utf-8'))
    # c.ReaderAIDA_create().read(iss, aobjects)
    c.ReaderAIDA_create().read_from_file(filename.encode('utf-8'), aobjects)
    return _aobjects_to_dict(&aobjects, patterns, unpatterns) if asdict \
        else _aobjects_to_list(&aobjects, patterns, unpatterns)


##
## Writers
##

def write(ana_objs, filename):
    """
    Write data objects to the provided filename,
    auto-determining the format from the file extension.
    """
    # cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a
    aolist = [ao for key,ao in sorted(ana_objs.items())] if hasattr(ana_objs, "items") \
              else ana_objs if hasattr(ana_objs, "__iter__") else [ana_objs]
    for a in aolist:
        vec.push_back(a.aoptr())
    c.IO_write_to_file(filename.encode('utf-8'), vec)
    #_str_to_file(oss.str(), filename)


def writeYODA(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in YODA format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a
    aolist = ana_objs.values() if hasattr(ana_objs, "values") else ana_objs \
             if hasattr(ana_objs, "__iter__") else [ana_objs]
    for a in aolist:
        vec.push_back(a.aoptr())
    if type(file_or_filename) is str:
        c.WriterYODA_create().write_to_file(file_or_filename.encode('utf-8'), vec)
    else:
        c.WriterYODA_create().write(oss, vec)
        _str_to_file(oss.str(), file_or_filename)


def writeFLAT(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in FLAT format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a
    aolist = ana_objs.values() if hasattr(ana_objs, "values") else ana_objs \
             if hasattr(ana_objs, "__iter__") else [ana_objs]
    for a in aolist:
        vec.push_back(a.aoptr())
    if type(file_or_filename) is str:
        c.WriterFLAT_create().write_to_file(file_or_filename.encode('utf-8'), vec)
    else:
        c.WriterFLAT_create().write(oss, vec)
        _str_to_file(oss.str(), file_or_filename)


def writeAIDA(ana_objs, file_or_filename):
    """
    Write data objects to the provided file in AIDA format.
    """
    cdef c.ostringstream oss
    cdef vector[c.AnalysisObject*] vec
    cdef AnalysisObject a
    aolist = ana_objs.values() if hasattr(ana_objs, "values") else ana_objs \
             if hasattr(ana_objs, "__iter__") else [ana_objs]
    for a in aolist:
        vec.push_back(a.aoptr())
    if type(file_or_filename) is str:
        c.WriterAIDA_create().write_to_file(file_or_filename.encode('utf-8'), vec)
    else:
        c.WriterAIDA_create().write(oss, vec)
        _str_to_file(oss.str(), file_or_filename)

##
## Indexers
##

class Index:
    """
    Index of a file.
    """
    def __init__(self, index_map, repr_string):
        self.index_map = index_map
        self.repr_string = repr_string

    def toDict(self):
        """
        Get dictionary containing index of the file.

        Returns:
            dict of str: dict of str: int : dictionary which holds
                analysis object types (str) as keys and
                dictionaries as values which in turn hold
                paths (str) as keys and bin counts (int) as values.

        Example:
            {
                "Histo1D": {
                    "/RAW/ATLAS_2017_I1514251/d21-x01-y01": 5,
                    "/RAW/ATLAS_2017_I1514251/d33-x01-y01": 16
                },
                "Scatter2D": {
                    "/ATLAS_2017_I1514251/d09-x01-y01": 3
                }
            }
        """
        return self.index_map

    def __repr__(self):
        """
        Get string representation of file index.

        Returns:
            str: formatted string.
                Example:
                    "
                    OBJECT TYPE: Scatter1D
                        ----------
                        PATH:      /RAW/_XSEC[WeightNormalisation]
                        BIN COUNT: 1
                        ----------
                    "
        """
        return self.repr_string.replace("\\n","\n")


cdef dict mkIndex(c.Reader& reader, string filename):
    """Generate index for file.

    Generates index object for specified filename, extracts
    map and string representation from c.Index object, converts
    index map to Python dictionary.

    Args:
        reader (c.Reader): object supporting Reader interface
        filename (str):    path to a file for index generation

    Returns:
        dict of str: dict of str: dict of str: int  : Dictionary containing
            two keys ["dict", "str"].

            Value of "dict" key contains dictionary which holds
                analysis object types (str) as keys and
                dictionaries as values which in turn hold
                paths (str) as keys and bin counts (int) as values.

            Value of "str" key contains string representation (str) of index map.
    """
    cdef c.Index idx = c.ReaderYODA_create().make_index(filename)
    cdef unordered_map[string,unordered_map[string,int]] AOidx

    AOidx = idx.getAOIndex()
    indexDict = _idxMap_to_dict(&AOidx)
    indexStr = idx.toString()

    return {"dict" : indexDict, "str" : indexStr}

def mkIndexYODA(filename):
    """
    Generate Index object for the provided YODA-format file.
    """
    if type(filename) is str:
      filename=filename.encode('utf-8')
    idx = mkIndex(c.ReaderYODA_create(), filename)
    return Index(idx["dict"], str(idx["str"]))
