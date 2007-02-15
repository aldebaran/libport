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
  AC_SUBST([OPENHRP_PATH], ['$(OPENHRP_PATH)'])
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


## Local Variables:
## mode: autoconf
## End:
