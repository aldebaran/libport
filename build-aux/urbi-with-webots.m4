dnl
# urbi-with-webots.m4: This file is part of build-aux.
# Copyright (C) Gostai S.A.S., 2006-2008.
dnl
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
dnl
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
dnl

# URBI_WITH_WEBOTS
# ----------
# Look for Webots.  Support --with-webots=PATH. Check the software
# location.
#
# Output variable WEBOTS_PATH point to the installation location.
# Set "$webots" to "true" or "false".
AC_DEFUN([URBI_WITH_WEBOTS],
[AC_ARG_WITH([webots],
	     [AC_HELP_STRING([--with-webots=webots-path],
			     [enable webots support [/usr/local/webots]])],
	     [], [with_webots=yes])

case $with_webots in
   no) webots=false
       ;;
  yes) webots=true
       WEBOTS_PATH=/usr/local/webots
       ;;
    *) webots=true
       WEBOTS_PATH=$with_webots
       ;;
esac

# Checking whether Webots is really there.
AC_MSG_CHECKING([for Webots])
if $webots; then
  if (test -f "$WEBOTS_PATH/webots" ||
      test -f "$WEBOTS_PATH/webots.exe" ||
      test -f "$WEBOTS_PATH/webots.app/Contents/MacOS/webots"); then
    AC_MSG_RESULT([$WEBOTS_PATH])
    webots=true
  else
    AC_MSG_RESULT([no, webots not found in $WEBOTS_PATH])
    webots=false
  fi
else
  AC_MSG_RESULT([no])
  webots=false
fi

if $webots; then
  # More tools for Webots.
  AC_SUBST([WEBOTS_CPPFLAGS], ['-I$(WEBOTS_PATH)/include'])
  AC_SUBST([WEBOTS_LDFLAGS],  ['-L$(WEBOTS_PATH)/lib -lController'])
fi

AM_CONDITIONAL([WITH_WEBOTS], [$webots])
AC_SUBST([WEBOTS_PATH])
])

AC_DEFUN([URBI_WITH_WEBOTS_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_WEBOTS])

   case $webots in
     '' | false)
       AC_MSG_ERROR([Failed to find Webots. Please install it or check config.log])
     ;;
   esac
])


## Local Variables:
## mode: autoconf
## End:
