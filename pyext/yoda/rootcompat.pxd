cimport cpython
from yoda.declarations cimport Histo1D, Profile1D, Scatter2D

cdef extern from "TObject.h":
    cdef cppclass TObject:
        pass

cdef extern from "TH1.h":
    cdef cppclass TH1(TObject):
        pass

    cdef cppclass TH1D(TObject):
        TH1D(TH1D)

cdef extern from "TProfile.h":
    cdef cppclass TProfile(TObject):
        TProfile(TProfile)

cdef extern from "TGraphAsymmErrors.h":
    cdef cppclass TGraphAsymmErrors(TObject):
        TGraphAsymmErrors(TGraphAsymmErrors)


cdef extern from "TH2.h":
    cdef cppclass TH2(TObject):
        pass

cdef extern from "pyroot_helpers.hh":
    cpython.PyObject* root_to_py_owned(TObject* root_obj)

cdef extern from "YODA/ROOTCnv.h" namespace "YODA":
    Scatter2D toScatter2D(TH1*)
    TH1D toTH1D(Histo1D)
    TProfile toTProfile(Profile1D)
    TGraphAsymmErrors toTGraph(Scatter2D)
