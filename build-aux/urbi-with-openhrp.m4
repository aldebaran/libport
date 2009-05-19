#
# urbi-with-openhrp.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# URBI_WITH_OPENHRP
# ----------
# Look for OpenHRP.  Support --with-openhrp=PATH. Check the software
# location.
#
# Output variable OPENHRP_PATH point to the installation location.
# Set "$openhrp" to "true" or "false".
AC_DEFUN([URBI_WITH_OPENHRP],
[AC_ARG_WITH([openhrp],
	     [AC_HELP_STRING([--with-openhrp=openhrp-path],
			     [enable openhrp support [$HOME/src/OpenHRP]])],
	     [], [with_openhrp=yes])

case $with_openhrp in
   no) openhrp=false
       ;;
  yes) openhrp=true
       OPENHRP_PATH=$HOME/src/OpenHRP
       ;;
    *) openhrp=true
       OPENHRP_PATH=$with_openhrp
       ;;
esac

# Checking whether OpenHRP is really there.
AC_MSG_CHECKING([for OpenHRP])
if $openhrp; then
  if (test -d "$OPENHRP_PATH"); then
    AC_MSG_RESULT([$OPENHRP_PATH])
    openhrp=true
  else
    AC_MSG_RESULT([no, openhrp not found in $OPENHRP_PATH])
    openhrp=false
  fi
else
  AC_MSG_RESULT([no])
  openhrp=false
fi

if $openhrp; then
  # More tools for OpenHRP.
  AC_SUBST([OPENHRP_COMMON_DIR],  ['$(OPENHRP_PATH)/Common'])
  AC_SUBST([OPENHRP_CONTROLLER_DIR],  ['$(OPENHRP_PATH)/Controller'])
fi

AM_CONDITIONAL([WITH_OPENHRP], [$openhrp])
AC_SUBST([OPENHRP_PATH])
])

AC_DEFUN([URBI_WITH_OPENHRP_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_OPENHRP])

   case $openhrp in
     '' | false)
       AC_MSG_ERROR([Failed to find OpenHRP. Please install it or check config.log])
     ;;
   esac
])


# URBI_WITH_ROBOT
# ----------
# Defines the name of the robot in the OpenHRP framework.
AC_DEFUN([URBI_WITH_ROBOT],
[AC_ARG_WITH([robot],
	     [AC_HELP_STRING([--with-robot=robot-name],
			     [define robot name [HRP2JRL]])],
	     [], [with_robot=HRP2JRL])

AC_SUBST([ROBOT], [$with_robot])
])


## Local Variables:
## mode: autoconf
## End:
