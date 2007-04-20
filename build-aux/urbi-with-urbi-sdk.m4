# URBI_WITH_URBI_SDK
# ----------
# Look for urbi-sdk.  Support --with-urbi-sdk=PATH. Check for
# urbi-sdk.
#
# Output variable URBI_SDK_PATH point to the installation location.
# Set "$urbi_sdk" to "true" or "false".
AC_DEFUN([URBI_WITH_URBI_SDK],
[AC_ARG_WITH([urbi_sdk],
	     [AC_HELP_STRING([--with-urbi-sdk=urbi-sdk-path],
			     [enable urbi-sdk support [/usr/local]])],
	     [], [with_urbi_sdk=yes])

case $with_urbi_sdk in
   no) urbi_sdk=false
       ;;
  yes) urbi_sdk=true
       URBI_SDK_PATH=/usr/local
       ;;
    *) urbi_sdk=true
       URBI_SDK_PATH=$with_urbi_sdk
       ;;
esac

# Checking whether Urbi_Sdk is really there.
AC_MSG_CHECKING([for urbi-sdk])
if $urbi_sdk; then
  if test -f "$URBI_SDK_PATH/lib/liburbi.la"; then
    AC_MSG_RESULT([$URBI_SDK_PATH])
    urbi_sdk=true
  else
    AC_MSG_RESULT([no, urbi-sdk not found in $URBI_SDK_PATH])
    urbi_sdk=false
  fi
else
  AC_MSG_RESULT([no])
  urbi_sdk=false
fi

if $urbi_sdk; then
  # More tools for Urbi_Sdk.
  AC_SUBST([URBI_SDK_CPPFLAGS], ['-I$(URBI_SDK_PATH)/include'])
  AC_SUBST([URBI_SDK_LDFLAGS],  ['-L$(URBI_SDK_PATH)/lib'])
  AC_SUBST([URBI_SDK_LIBS],  ['$(URBI_SDK_PATH)/lib/liburbi.la'])
fi

AM_CONDITIONAL([WITH_URBI_SDK], [$urbi_sdk])
AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
  ["$DISTCHECK_CONFIGURE_FLAGS --with-urbi-sdk='$URBI_SDK_PATH'"])
AC_SUBST([URBI_SDK_PATH])
])

AC_DEFUN([URBI_WITH_URBI_SDK_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_URBI_SDK])

   case "$urbi_sdk" in
    '' | 'false')
     AC_MSG_ERROR(
      [Failed to find urbi-sdk. Please install it or check config.log])
    ;;
   esac
])


## Local Variables:
## mode: autoconf
## End:
