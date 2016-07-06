#ifndef __PYX_HAVE__yoda__core
#define __PYX_HAVE__yoda__core


#ifndef __PYX_HAVE_API__yoda__core

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

#ifndef DL_IMPORT
  #define DL_IMPORT(_T) _T
#endif

__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_Exception;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_BinningError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_RangeError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_LockError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_GridError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_LogicError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_LowStatsError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_WeightError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_AnnotationError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_ReadError;
__PYX_EXTERN_C DL_IMPORT(PyObject) *YodaExc_UserError;

#endif /* !__PYX_HAVE_API__yoda__core */

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initcore(void);
#else
PyMODINIT_FUNC PyInit_core(void);
#endif

#endif /* !__PYX_HAVE__yoda__core */
