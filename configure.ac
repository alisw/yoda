## Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([YODA],[1.6.3],[yoda@projects.hepforge.org],[YODA])

## Check and block installation into the src/build dir
if test "$prefix" = "$PWD"; then
  AC_MSG_ERROR([Installation into the build directory is not supported: use a different --prefix argument])
fi
## Force default prefix to have a path value rather than NONE
if test "$prefix" = "NONE"; then
   prefix=/usr/local
fi

dnl AC_CONFIG_SRCDIR([src/Bin1D.cc])
AM_INIT_AUTOMAKE([subdir-objects -Wall dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_HEADERS([include/YODA/Config/DummyConfig.h include/YODA/Config/YodaConfig.h include/YODA/Config/BuildConfig.h])
AC_DEFINE_UNQUOTED(YODA_VERSION, "$PACKAGE_VERSION", "YODA version string")
AC_DEFINE_UNQUOTED(YODA_NAME, "$PACKAGE_NAME", "YODA name string")
AC_DEFINE_UNQUOTED(YODA_STRING, "$PACKAGE_STRING", "YODA name and version string")
AC_DEFINE_UNQUOTED(YODA_TARNAME, "$PACKAGE_TARNAME", "YODA short name string")
AC_DEFINE_UNQUOTED(YODA_BUGREPORT, "$PACKAGE_BUGREPORT", "YODA contact email address")

## OS X
AC_CEDAR_OSX

## Set default compiler flags
if test "x$CXXFLAGS" = "x"; then CXXFLAGS="-O3"; fi

## Compiler setup
AC_LANG(C++)
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])

AC_PROG_INSTALL
AC_PROG_LN_S
AC_DISABLE_STATIC
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

## Work out library suffix for the build
LIB_SUFFIX=\\\"$shrext_cmds\\\"
AC_SUBST([LIB_SUFFIX])

## Set default build flags
AC_CEDAR_CHECKCXXFLAG([-pedantic], [AM_CXXFLAGS="$AM_CXXFLAGS -pedantic"])
AC_CEDAR_CHECKCXXFLAG([-Wall], [AM_CXXFLAGS="$AM_CXXFLAGS -Wall -Wno-format"])
dnl AC_CEDAR_CHECKCXXFLAG([-std=c++98], [AM_CXXFLAGS="$AM_CXXFLAGS -std=c++98"])
dnl AC_CEDAR_CHECKCXXFLAG([-Wno-unused-variable], [AM_CXXFLAGS="$AM_CXXFLAGS -Wno-unused-variable"])

## Debug flag (default=none)
AC_ARG_ENABLE([debug], [AC_HELP_STRING(--enable-debug, [build with debugging symbols  @<:@default=no@:>@])], [], [enable_debug=no])
if test x$enable_debug = xyes; then
  [AM_CXXFLAGS="$AM_CXXFLAGS -g"]
fi


## Optional ROOT compatibility
AC_ARG_ENABLE([root], [AC_HELP_STRING(--disable-root,
  [try to build YODA interface to PyROOT (needs root-config) @<:@default=yes@:>@])], [enable_root=no], [])
if test "x$enable_root" = "xyes"; then
  AC_PATH_PROG(ROOTCONFIG, [root-config])
  if test "x$ROOTCONFIG" = "x"; then
    AC_MSG_WARN([root-config not found -- not building extra ROOT compatibility tools])
    enable_root=no;
  else
    AC_MSG_CHECKING([ROOT version])
    ROOT_VERSION=`$ROOTCONFIG --version`
    ROOT_MAJOR_VERSION=`echo $ROOT_VERSION | cut -d. -f1`
    ROOT_MINOR_VERSION=`echo $ROOT_VERSION | cut -d. -f2 | cut -d/ -f1`
    ROOT_MICRO_VERSION=`echo $ROOT_VERSION | cut -d. -f2 | cut -d/ -f2`
    AC_MSG_RESULT([$ROOT_VERSION ($ROOT_MAJOR_VERSION,$ROOT_MINOR_VERSION,$ROOT_MICRO_VERSION)])
    if test "$ROOT_MAJOR_VERSION" -lt 5; then
      enable_root=no;
      AC_MSG_WARN([ROOT major version is < 5 -- not building extra ROOT compatibility tools])
    elif test "$ROOT_MAJOR_VERSION" -eq 5; then
      if test "$ROOT_MINOR_VERSION" -lt 33; then
        enable_root=no;
        AC_MSG_WARN([ROOT version is less than 5.33 -- not building extra ROOT compatibility tools])
      elif test "$ROOT_MINOR_VERSION" -eq 33 && test "$ROOT_MICRO_VERSION" -lt 2; then
        enable_root=no;
        AC_MSG_WARN([ROOT version is less than 5.33/02 -- not building extra ROOT compatibility tools])
      fi
    fi
    # TODO: Test for existence of TPython, instance_from_void API, etc.
    #AM_CXXFLAGS="$AM_CXXFLAGS -Wno-long-long"
    ROOT_CXXFLAGS=`$ROOTCONFIG --cflags`
    ROOT_LDFLAGS=`$ROOTCONFIG --ldflags`
    ROOT_LIBS=`$ROOTCONFIG --libs`
    AC_SUBST(ROOT_CXXFLAGS)
    AC_SUBST(ROOT_LDFLAGS)
    AC_SUBST(ROOT_LIBS)
  fi
fi
AM_CONDITIONAL(ENABLE_ROOT, [test x$enable_root = xyes])
if test x$enable_root = xyes; then
  AC_MSG_NOTICE([Building extra ROOT compatibility tools])
else
  AC_MSG_NOTICE([Not building extra ROOT compatibility tools])
fi


## Python extension
AC_ARG_ENABLE(pyext, [AC_HELP_STRING(--disable-pyext,
  [don't build Python module (default=build)])],
  [], [enable_pyext=yes])
AZ_PYTHON_DEFAULT
## Basic Python checks
if test x$enable_pyext = xyes; then
  AZ_PYTHON_PATH
  AZ_PYTHON_VERSION_ENSURE([2.5])
  PYTHON_VERSION=`$PYTHON -c "import sys; print '.'.join(map(str, sys.version_info@<:@:2@:>@));"`
  AC_SUBST(PYTHON_VERSION)
  YODA_PYTHONPATH=`$PYTHON -c "import distutils.sysconfig; print distutils.sysconfig.get_python_lib(prefix='$prefix', plat_specific=True);"`
  AC_SUBST(YODA_PYTHONPATH)
  ## Test for Python header
  if test -x "$PYTHON"; then
    AC_MSG_CHECKING([for Python include path])
    python_incpath=`$PYTHON -c "import distutils.sysconfig; print distutils.sysconfig.get_python_inc();"`
    AC_MSG_RESULT([$python_incpath])
    python_header="$python_incpath/Python.h"
    if test -z "$python_incpath"; then
     AC_MSG_ERROR([Can't build Python extension since include directory cannot be determined from distutils])
      enable_pyext=no
    elif test ! -e "$python_header"; then
      AC_MSG_ERROR([Can't build Python extension since header file $python_header cannot be found])
      enable_pyext=no
    fi
  else
    AC_MSG_ERROR([Can't build Python extension since python can't be found])
    enable_pyext=no
  fi
fi
## Finish Python extension testing
if test x$enable_pyext == xyes; then
  ## Set extra Python extension build flags (to cope with Cython output code oddities)
  PYEXT_CXXFLAGS=$CXXFLAGS
  AC_CEDAR_CHECKCXXFLAG([-Wno-unused-but-set-variable], [PYEXT_CXXFLAGS="$PYEXT_CXXFLAGS -Wno-unused-but-set-variable"])
  AC_CEDAR_CHECKCXXFLAG([-Wno-sign-compare], [PYEXT_CXXFLAGS="$PYEXT_CXXFLAGS -Wno-sign-compare"])
  AC_CEDAR_CHECKCXXFLAG([-Wno-strict-prototypes], [PYEXT_CXXFLAGS="$PYEXT_CXXFLAGS -Wno-strict-prototypes"])
  AC_SUBST(PYEXT_CXXFLAGS)
  AC_MSG_NOTICE([All Python build checks successful: 'yoda' Python extension will be built])
fi
AM_CONDITIONAL(ENABLE_PYEXT, [test x$enable_pyext == xyes])


## Cython checks
if test x$enable_pyext == xyes; then
  AM_CHECK_CYTHON([0.20], [:], [:])
  if test x$CYTHON_FOUND = xyes; then
    AC_MSG_NOTICE([Cython >= 0.18 found: Python extension source can be rebuilt (for developers)])
  fi
fi
AM_CONDITIONAL(WITH_CYTHON, [test x$CYTHON_FOUND = xyes])


## Extend and substitute the default build flags after lib testing
AM_CPPFLAGS="-I\$(top_srcdir)/include -I\$(top_builddir)/include"
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_CXXFLAGS)


## Build Doxygen if possible
AC_PATH_PROG(DOXYGEN, doxygen)
AM_CONDITIONAL(WITH_DOXYGEN, test "$DOXYGEN")


## Build file output
AC_EMPTY_SUBST
AC_CONFIG_FILES([Makefile Doxyfile])
AC_CONFIG_FILES([include/Makefile include/YODA/Makefile])
AC_CONFIG_FILES([src/Makefile src/tinyxml/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([pyext/Makefile pyext/setup.py pyext/yoda/Makefile])
AC_CONFIG_FILES([bin/Makefile bin/yoda-config])
AC_CONFIG_FILES([yodaenv.sh yoda.pc])

AC_OUTPUT

if test x$enable_pyext == xyes; then
   cat <<EOF

************************************************************
YODA CONFIGURED!

Now build and install (to the $prefix tree) with e.g.
make -j2 && make -j2 install
************************************************************
EOF
fi
