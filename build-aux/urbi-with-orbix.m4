dnl
dnl urbi-with-orbix.m4: This file is part of build-aux.
dnl Copyright (C) Gostai S.A.S., 2006-2008.
dnl
dnl This software is provided "as is" without warranty of any kind,
dnl either expressed or implied, including but not limited to the
dnl implied warranties of fitness for a particular purpose.
dnl
dnl See the LICENSE file for more information.
dnl For comments, bug reports and feedback: http://www.urbiforge.com
dnl

# URBI_WITH_ORBIX
# ----------
# Look for ORBIX.  Support --with-ORBIX=PATH. Check the software
# location.
#
# Output variable ORBIX_PATH point to the installation location.
# Set "$ORBIX" to "true" or "false".
AC_DEFUN([URBI_WITH_ORBIX],
[AC_ARG_WITH([orbix],
	     [AC_HELP_STRING([--with-orbix=orbix-path],
			     [enable ORBIX support [/usr/local/IONA/OBE/2.2n]])],
	     [], [with_orbix=yes])

case $with_orbix in
   no) ORBIX=false
       ;;
  yes) ORBIX=true
       ORBIX_PATH=/usr/local/IONA/OBE/2.2n
       ;;
    *) ORBIX=true
       ORBIX_PATH=$with_orbix
       ;;
esac

# Checking whether ORBIX is really there.
AC_MSG_CHECKING([for ORBIX])
if $ORBIX; then
  if (test -d "$ORBIX_PATH"); then
    AC_MSG_RESULT([$ORBIX_PATH])
    ORBIX=true
  else
    AC_MSG_RESULT([no, ORBIX not found in $ORBIX_PATH])
    ORBIX=false
  fi
else
  AC_MSG_RESULT([no])
  ORBIX=false
fi

if $ORBIX; then
  # More tools for ORBIX.
  AC_SUBST([ORBIX_INCLUDE],
    ['-I$(ORBIX_PATH)/acpp/include -I$(ORBIX_PATH)/c/include'])
  AC_SUBST([ORBIX_ORB], ['ORBIXE'])
  AC_SUBST([ORBIX_VERSION], ['2'])

  AC_SUBST([ORBIX_CPPFLAGS],
    ['$(ORBIX_INCLUDE) -D$(ORBIX_ORB) -DORBIX_VERSION=$(ORBIX_VERSION)'])
fi

AM_CONDITIONAL([WITH_ORBIX], [$ORBIX])
AC_SUBST([ORBIX_PATH])
])

AC_DEFUN([URBI_WITH_ORBIX_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_ORBIX])

   case $ORBIX in
     '' | false)
       AC_MSG_ERROR([Failed to find ORBIX. Please install it or check config.log])
     ;;
   esac
])


## Local Variables:
## mode: autoconf
## End:
