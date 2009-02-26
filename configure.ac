## ------------- ##
## Package setup ##
## ------------- ##

AC_INIT([Libport], [UNDEFINED], [common-bugs@lists.gostai.com])
URBI_PACKAGE_VERSION
URBI_PACKAGE_COPYRIGHT([Gostai SAS], [2005-2009])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux])
URBI_DISTCHECK_CONFIGURE_FLAGS

# Automake 1.10 generates incomplete "clean-libtool" targets which
# result in the end in failing distchecks.
AM_INIT_AUTOMAKE([1.10.2 foreign -Wall nostdinc subdir-objects])

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

# For some reason, coroutine don't work properly on OSX PPC.  The test
# suite seems ok though, so maybe we could make it work.  Meanwhile,
# let's work with threads.
case $host in
  (powerpc-apple-darwin*)
     SCHED_CORO_OSTHREAD=true
     AC_DEFINE([SCHED_CORO_OSTHREAD], [1]);;
  (*) SCHED_CORO_OSTHREAD=false;;
esac
AM_CONDITIONAL([SCHED_CORO_OSTHREAD], [$SCHED_CORO_OSTHREAD])

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
