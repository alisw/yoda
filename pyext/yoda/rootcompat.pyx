cimport rootcompat as c
import yoda
cimport yoda.declarations as cyoda
cimport yoda.util
import ROOT
cimport cython.operator.dereference as deref
#ROOT.PyConfig.IgnoreCommandLineOptions = True

# We should find a way to restructure the YODA bindings to allow external Cython
# programs to get the used structs -- this requires using pxd files accordingly.


#def convert_yoda():
    #cdef yoda.util.Base h = yoda.Histo1D(10, 0, 1, '/test')
    #cdef cyoda.Histo1D* hptr = <cyoda.Histo1D*> h.ptr()
    #cdef TObject* thist = new TH1D(toTH1D(hptr[0]))
    #return <object> (c.root_to_py_owned(thist))

#def toScatter2D(TH1*):

# Dispatch on python typename

def to_root(yoda.util.Base yoda_obj):

    cdef void *ptr = yoda_obj.ptr()

    if isinstance(yoda_obj, yoda.Histo1D):
        return _toTH1D(<cyoda.Histo1D*> ptr)

    elif isinstance(yoda_obj, yoda.Profile1D):
        return _toTProfile(<cyoda.Profile1D*> ptr)

    elif isinstance(yoda_obj, yoda.Scatter2D):
        return _toTGraph(<cyoda.Scatter2D*> ptr)

cdef object root_to_py(c.TObject* tobj):
    return <object> c.root_to_py_owned(tobj)

cdef _toTH1D(cyoda.Histo1D* h1d):
    return ROOT.TH1D(
        root_to_py(new c.TH1D(c.toTH1D(deref(h1d)))))

cdef _toTProfile(cyoda.Profile1D* p1d):
    return ROOT.TProfile(
        root_to_py(new c.TProfile(c.toTProfile(deref(p1d)))))

cdef _toTGraph(cyoda.Scatter2D* s2d):
    return ROOT.TGraphAsymmErrors(
        root_to_py(new c.TGraphAsymmErrors(c.toTGraph(deref(s2d)))))
