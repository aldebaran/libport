# This file is part of Urbi SDK.
# Copyright (C) 2006-2012, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.org
#

## ------------- ##
## Package setup ##
## ------------- ##

AC_INIT([Libport], [UNDEFINED], [libport-bugs@lists.gostai.com])
URBI_PACKAGE_VERSION
URBI_PACKAGE_COPYRIGHT([Gostai S.A.S.], [2005-2012])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux])
URBI_DISTCHECK_CONFIGURE_FLAGS
URBI_PACKAGE_KIND([sdk])

AM_INIT_AUTOMAKE([1.11 foreign -Wall nostdinc silent-rules subdir-objects])
AM_SILENT_RULES([yes])

## -------------------- ##
## Tool/Compilers setup ##
## -------------------- ##

URBI_COMPILATION_MODE
# Look for a C++ compiler.
URBI_PROG_CXX
AC_LANG([C++])
URBI_DIRS([libport])

# We have some assembly for coroutine support.
AM_PROG_AS

URBI_WIN32
URBI_SOCKETS
URBI_LIBTOOL
URBI_SHLIBEXT
# Name of the subdirectory that contains libltdl sources.
LT_CONFIG_LTDL_DIR([libltdl])
LTDL_INIT([convenience nonrecursive])
# We want the debug messages.
AC_DEFINE([LT_DEBUG_LOADERS], [1],
          [Define to have libltdl support log messages.])

AC_CONFIG_HEADERS([include/config.h])

# Basically, the following lines states we install in prefix.
URBI_PACKAGE_KIND([sdk])

URBI_BOOST_REQUIRE([1.38])
URBI_BOOST([FILESYSTEM])
URBI_BOOST([SIGNALS])
URBI_BOOST([SYSTEM])
URBI_BOOST([TEST])
URBI_BOOST([THREADS])

URBI_LIBPORT

## --------- ##
## Libport.  ##
## --------- ##

# libport/asio.
URBI_ARG_ENABLE([enable-ssl], [enable SSL in Libport.Asio],
                [yes|no], [no])
if test x$enable_ssl = xyes; then
  AC_DEFINE([ENABLE_SSL], [1],
            [Define to 1 to enable SLL support.])

  # -lssl suffices on GNU/Linux, OS X wants -lcrypto explicitly.
  AC_SUBST([SSL_LIBS], ['-lssl -lcrypto'])
fi

# libport/backtrace.
AC_CHECK_HEADERS([execinfo.h])

# libport/semaphore.
AC_CHECK_LIB([rt], [clock_gettime],
             [AC_SUBST([CLOCK_LIBS], [-lrt])])

# libport/sys/prctl.h
AC_CHECK_HEADERS([sys/prctl.h])

# libport/sys/utsname
AC_CHECK_HEADERS([sys/utsname.h])

# libport/sys/wait.h
AC_CHECK_HEADERS([sys/wait.h])

# libport/valgrind.
AC_CHECK_HEADERS([valgrind/valgrind.h])

## ---------- ##
## Libsched.  ##
## ---------- ##

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
URBI_PTHREAD_SOURCES

# AutoTroll with Qt.
# AT_WITH_QT
# AC_CONFIG_FILES([gui/Makefile])
URBI_ARG_ENABLE([enable-boost-serialization],
                [enable boost::serialization support],
                [yes|no], [no])
AM_CONDITIONAL([WITH_BOOST_SERIALIZATION],
               [test x$enable_boost_serialization = xyes])
if test x$enable_boost_serialization = xyes; then
  AC_DEFINE([WITH_BOOST_SERIALIZATION], [1],
            [Define to enable boost::serialization support.])
  URBI_BOOST([SERIALIZATION])
fi

# Multithread support in sched
URBI_ARG_ENABLE([enable-sched-multithread],
                [enable multithread support in libsched],
                [yes|no], [no])
if test x$enable_sched_multithread = xyes; then
  AC_DEFINE([LIBPORT_SCHED_MULTITHREAD], [1],
            [Define to 1 to enable multithread support in libsched.])
fi

WITH_XENOMAI
URBI_LIB_SUFFIX
AC_CONFIG_FILES([Makefile share/pkgconfig/libport.pc])

URBI_CONFIG_SCRIPTS([doc/doxygen.sh])

AC_OUTPUT
