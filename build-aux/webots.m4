# WEBOTS
# ----------
# Look for Webots.  Support --with-webots=PATH. Check the software
# location.
#
# Output variable WEBOTS_PATH point to the installation location.
# Set "$webots" to "true" or "false".
AC_DEFUN([WEBOTS],
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
  if test -f $WEBOTS_PATH/webots -o \
	  -f $WEBOTS_PATH/webots.exe -o \
	  -f $WEBOTS_PATH/webots.app/Contents/MacOS; then
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

AM_CONDITIONAL([WEBOTS], [$webots])
AC_SUBST([WEBOTS_PATH])
])

AC_DEFUN([WEBOTS_REQUIRED],
[
   AC_REQUIRE([WEBOTS])

   if test -z $webots -o $webots = false; then
      AC_MSG_ERROR([Failed to find Webots. Please install it or check config.log])
   fi
])


## Local Variables:
## mode: autoconf
## End:
