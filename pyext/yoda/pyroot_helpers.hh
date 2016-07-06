#include "YODA/ROOTCnv.h"
#include "Python.h"
#include "YODA/Profile1D.h"
#include "TPython.h"

// One useful shim to help us
inline PyObject* root_to_py_owned(TObject* root_obj) {
  return TPython::ObjectProxy_FromVoidPtr(root_obj, root_obj->ClassName());
  /// @todo Different signatures in different ROOT versions?
  //return TPython::ObjectProxy_FromVoidPtr(root_obj, root_obj->ClassName(), kFALSE);
}
