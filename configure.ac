## ------------- ##
## Package setup ##
## ------------- ##

AC_INIT([Libport], [UNDEFINED], [common-bugs@lists.gostai.com])
URBI_PACKAGE_VERSION
URBI_PACKAGE_COPYRIGHT([Gostai SAS], [2005-2009])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign -Wall nostdinc subdir-objects])

## -------------------- ##
## Tool/Compilers setup ##
## -------------------- ##

# Look for a C++ compiler.
URBI_PROG_CXX

# We have some assembly for coroutine support.
AM_PROG_AS

URBI_LIBTOOL
# Name of the subdirectory that contains libltdl sources.
LT_CONFIG_LTDL_DIR([libltdl])
LTDL_INIT([nonrecursive])

AC_CONFIG_HEADERS([include/config.h])

# Basically, the following lines states we install in prefix.
URBI_PACKAGE_KIND([sdk])
URBI_LIBPORT

BOOST_TEST
BOOST_SYSTEM

URBI_COMPILATION_MODE

# AutoTroll with Qt.
# AT_WITH_QT
# AC_CONFIG_FILES([gui/Makefile])
URBI_ARG_ENABLE([enable-boost-serialization],
                [Enable boost::serialization support],
                [yes|no], [no])
AM_CONDITIONAL([WITH_BOOST_SERIALIZATION],
               [test x$enable_boost_serialization = xyes])
if test x$enable_boost_serialization = xyes; then
  AC_DEFINE([WITH_BOOST_SERIALIZATION], [1],
            [Define to enable boost::serialization support.])
  BOOST_SERIALIZATION
fi


AC_CONFIG_FILES([Makefile])

AC_CONFIG_FILES([doc/doxygen.sh],    [chmod a=rx doc/doxygen.sh])
AC_CONFIG_FILES([build-aux/test.sh], [chmod a=rx build-aux/test.sh])

AC_OUTPUT
