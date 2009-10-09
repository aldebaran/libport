#						-*- Autoconf -*-

#
# urbi-pthread.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# URBI_PTHREAD
# ------------
# Look for the pthreads.
AC_DEFUN([URBI_PTHREAD],
[AC_REQUIRE([URBI_WIN32])dnl
ACX_PTHREAD([pthreads=true], [pthreads=false])
AC_CHECK_HEADERS([pthread.h])

SDK_CFLAGS="$SDK_CFLAGS $PTHREAD_CFLAGS"
SDK_CXXFLAGS="$SDK_CXXFLAGS $PTHREAD_CFLAGS"
SDK_LIBS="$SDK_LIBS $PTHREAD_LIBS"
if ! $windows; then
   PTHREAD_LDFLAGS="$PTHREAD_LDFLAGS -lpthread"
fi
AC_SUBST([PTHREAD_LDFLAGS])
])
