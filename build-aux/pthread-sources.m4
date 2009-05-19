#
# pthread-sources.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#


AC_DEFUN([URBI_PTHREAD_SOURCES],
[ AC_ARG_WITH([pthread_sources],
	[AC_HELP_STRING([--with-pthread-sources=dir],
	  [Location of the libpthread sources])],
	  [],[with_pthread_sources=no])
case $with_pthread_sources in
 (no) have_pthread_sources=false ;;
 (*) PTHREAD_SOURCES=$with_pthread_sources; have_pthread_sources=true ;;
esac

AC_MSG_CHECKING([for pthread sources])
if $have_pthread_sources; then
  if test -f "$PTHREAD_SOURCES/linuxthreads/internals.h"; then
    AC_MSG_RESULT([$PTHREAD_SOURCES])
  else
    AC_MSG_ERROR([$PTHREAD_SOURCES/linuxthreads/internals.h not found])
  fi
else
  AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([PTHREAD_SOURCES], [$have_pthread_sources])

$have_pthread_sources && AC_DEFINE([HAVE_PTHREAD_SOURCES], 1, [Define to 1 if you have pthread sources. ])
AC_SUBST([PTHREAD_SOURCES])
])
