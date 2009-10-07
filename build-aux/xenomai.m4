#
# xenomai.m4: This file is part of build-aux.
# Copyright (C) 2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

AC_DEFUN([WITH_XENOMAI],
[AC_ARG_WITH([xenomai],
        [AC_HELP_STRING([--with-xenomai=xenomai-path],
        [enable xenomai support [no]])],
        [], [with_xenomai=no])

case $with_xenomai in
        no) xenomai=false ;;
        yes) xenomai=true XENOMAI_PATH=/usr ;;
        *) xenomai=true; XENOMAI_PATH=$with_xenomai ;;
esac

AC_MSG_CHECKING([for xenomai])
if $xenomai; then
  if test -f $XENOMAI_PATH/bin/xeno-config; then
    AC_MSG_RESULT([ $XENOMAI_PATH])
  else
    AC_MSG_RESULT([no, xeno-config not found in $XENOMAI_PATH/bin/])
    xenomai=false
  fi
else
  AC_MSG_RESULT([no])
fi

if $xenomai; then
  XENOMAI_POSIX_CPPFLAGS=$($XENOMAI_PATH/bin/xeno-config --posix-cflags)
  echo cflags $XENOMAI_POSIX_CPPFLAGS
  AC_SUBST([XENOMAI_POSIX_CPPFLAGS])
  XENOMAI_POSIX_LDFLAGS=$($XENOMAI_PATH/bin/xeno-config --posix-ldflags)
  AC_SUBST([XENOMAI_POSIX_LDFLAGS])
  AC_SUBST([XENOMAI_PATH])
  CPPFLAGS="$CPPFLAGS $XENOMAI_POSIX_CPPFLAGS"
  LDFLAGS="$LDFLAGS $XENOMAI_POSIX_LDFLAGS"
  AC_MSG_NOTICE(["*** xenomai override flags in place ***])
fi

AM_CONDITIONAL([WITH_XENOMAI], [$xenomai])

])


