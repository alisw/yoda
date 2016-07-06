#include "YODA/ROOTCnv.h"
#include "Python.h"
#include "YODA/Profile1D.h"
#include "TPython.h"


// Get a PyROOT object from a ROOT one
inline PyObject* root_to_py_owned(TObject* root_obj) {
  return TPython::ObjectProxy_FromVoidPtr(root_obj, root_obj->ClassName());
  /// @todo Different signatures in different ROOT versions?
  //return TPython::ObjectProxy_FromVoidPtr(root_obj, root_obj->ClassName(), kFALSE);
}


// Get the ROOT object in a PyROOT one
inline TObject* py_owned_to_root(PyObject* pyroot_obj) {
  return (TObject*) TPython::ObjectProxy_AsVoidPtr(pyroot_obj);
}
