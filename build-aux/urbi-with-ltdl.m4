##
## urbi-with-ltdl.m4: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# URBI_WITH_LTDL
# ----------
# Look for ltdl library.
#
# Set "$ltdl" to "true" or "false".
AC_DEFUN([URBI_WITH_LTDL],
[AC_ARG_WITH([ltdl],
	     [AC_HELP_STRING([--with-ltdl=yes|no],
			     [enable ltdl support])],
	     [], [with_ltdl=yes])

case $with_ltdl in
   no) ltdl=false
       ;;
  *) ltdl=true
       ;;
esac


if $ltdl; then
  AC_SUBST([LTDL_LIBS], ['-lltdl'])
  AC_DEFINE([WITH_LTDL], [1], [Define if you have ltdl])
  SDK_LIBS="$SDK_LIBS $LTDL_LIBS"
fi

])

AC_DEFUN([URBI_WITH_LTDL_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_LTDL])

  case "$ltdl" in
    '' | 'false')
     AC_MSG_ERROR(
      [Failed to find ltdl. Please install it or check config.log])
    ;;
   esac
])

