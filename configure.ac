## ------------- ##
## Package setup ##
## ------------- ##

AC_INIT([Libport], [UNDEFINED], [common-bugs@lists.gostai.com])
URBI_PACKAGE_VERSION
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign -Wall])

## -------------------- ##
## Tool/Compilers setup ##
## -------------------- ##

# Look for a C++ compiler.
URBI_PROG_CXX

AC_CONFIG_HEADERS([include/config.h])

# If you need to create a library, use libtool.
LT_PREREQ([2.2.6])
# Name the subdirectory that contains libltdl sources
LT_CONFIG_LTDL_DIR([libltdl])
LT_INIT([pic-only shared disable-static dlopen win32-dll])
# I don't know why I need this with Libtool 2.2.6
mkdir -p .deps/libltdl
LTDL_INIT([nonrecursive])
AC_SUBST([LIBTOOL_DEPS])

BOOST_TEST

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

## -------------- ##
## Sub packages.  ##
## -------------- ##

# Basically, the following lines states we install in prefix.
URBI_PACKAGE_KIND([sdk])
URBI_LIBPORT

AC_CONFIG_FILES([
  Makefile
      lib/libport/Makefile
    tests/Makefile
      tests/libport/Makefile
    doc/Makefile
])

AC_CONFIG_FILES([doc/doxygen.sh],    [chmod a=rx doc/doxygen.sh])
AC_CONFIG_FILES([build-aux/test.sh], [chmod a=rx build-aux/test.sh])

AC_OUTPUT
