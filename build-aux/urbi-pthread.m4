#						-*- Autoconf -*-

dnl
# urbi-pthread.m4: This file is part of build-aux.
# Copyright (C) Gostai S.A.S., 2006-2008.
dnl
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
dnl
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
dnl

# URBI_PTHREAD
# ------------
# Look for the pthreads, or for windows.  Define the automake conditional
# WIN32.
# FIXME: Deprecate/remove this macro.
AC_DEFUN([URBI_PTHREAD],
[ACX_PTHREAD([pthreads=true], [pthreads=false])
AC_CHECK_HEADERS([windows.h], [windows=true], [windows=false])
AC_CHECK_HEADERS([pthread.h])

# FIXME: This has *nothing* to do with pthreads, move somewhere else.
if $windows; then
  AC_SUBST([PTHREAD_LIBS], ['-lws2_32 -lgdi32'])
fi
AM_CONDITIONAL([WIN32], [$windows])

#if $windows || $pthreads; then :; else
#  AC_MSG_ERROR([pthreads or WIN32 API is required])
#fi

SDK_CFLAGS="$SDK_CFLAGS $PTHREAD_CFLAGS"
SDK_CXXFLAGS="$SDK_CXXFLAGS $PTHREAD_CFLAGS"
SDK_LIBS="$SDK_LIBS $PTHREAD_LIBS"
])
