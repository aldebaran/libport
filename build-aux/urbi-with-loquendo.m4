#
# urbi-with-loquendo.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# URBI_WITH_LOQUENDO
# ----------
# Look for loquendo.  Support --with-loquendo=PATH. Check the software
# location.
#
# Output variable LOQUENDO_PATH point to the installation location.
# Set "$loquendo" to "true" or "false".
AC_DEFUN([URBI_WITH_LOQUENDO],
[AC_ARG_WITH([loquendo],
	     [AC_HELP_STRING([--with-loquendo=loquendo-path],
			     [set the path to loquendo [/opt/Loquendo/LTTS7]])],
	     [], [with_loquendo=no])

case $with_loquendo in
   no) loquendo=false
       ;;
  yes) loquendo=true
       LOQUENDO_PATH=/opt/Loquendo/LTTS7
       ;;
    *) loquendo=true
       LOQUENDO_PATH=$with_loquendo
       ;;
esac

# Checking whether Loquendo is really there.
AC_MSG_CHECKING([for loquendo])
if $loquendo; then
  if (test -f "$LOQUENDO_PATH/bin/libLoqTTS7.so"); then
    AC_MSG_RESULT([$LOQUENDO_PATH])
    loquendo=true
  else
    AC_MSG_RESULT([no, loquendo not found in $LOQUENDO_PATH])
    loquendo=false
  fi
else
  AC_MSG_RESULT([no])
  loquendo=false
fi

if $loquendo; then
  LOQUENDO_CPPFLAGS="-I$LOQUENDO_PATH/include"
  LOQUENDO_LDFLAGS="-L$LOQUENDO_PATH/bin -lLoqTTS7"
  AC_SUBST([LOQUENDO_CPPFLAGS],[$LOQUENDO_CPPFLAGS])
  AC_SUBST([LOQUENDO_LDFLAGS],[$LOQUENDO_LDFLAGS])
fi

AM_CONDITIONAL([WITH_LOQUENDO], [$loquendo])
AC_SUBST([LOQUENDO_PATH])
])

AC_DEFUN([URBI_LOQUENDO_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_LOQUENDO])

   case $loquendo in
     '' | false)
       AC_MSG_ERROR([Failed to find loquendo. Please install it or check config.log])
     ;;
   esac
])


## Local Variables:
## mode: autoconf
## End:
