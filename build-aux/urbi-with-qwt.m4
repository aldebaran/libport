# URBI_WITH_QWT
# ----------
# Look for qwt.  Support --with-qwt=PATH. Check for
# qwt.
#
# Output variable QWT_PATH point to the installation location.
# Set "$qwt" to "true" or "false".
AC_DEFUN([URBI_WITH_QWT],
[AC_ARG_WITH([qwt],
	     [AC_HELP_STRING([--with-qwt=qwt-path],
			     [enable qwt support [/usr/local/qwt-5.0.2]])],
	     [], [with_qwt=yes])

case $with_qwt in
   no) qwt=false
       ;;
  yes) qwt=true
       QWT_PATH=/usr/local/qwt-5.0.2
       ;;
    *) qwt=true
       QWT_PATH=$with_qwt
       ;;
esac

# Checking whether QWT is really there.
AC_MSG_CHECKING([for qwt])
if $qwt; then
  if test -f "$QWT_PATH/lib/libqwt5.a"; then
    AC_MSG_RESULT([$QWT_PATH])
    qwt=true
  else
    AC_MSG_RESULT([no, qwt not found in $QWT_PATH])
    qwt=false
  fi
else
  AC_MSG_RESULT([no])
  qwt=false
fi

if $qwt; then
  # More tools for QWT.
  AC_SUBST([QWT_CPPFLAGS], ['-I$(QWT_PATH)/include'])
  AC_SUBST([QWT_LDFLAGS],  ['-L$(QWT_PATH)/lib'])
  AC_SUBST([QWT_LIBS],  ['$(QWT_PATH)/lib/libqwt5.a'])
fi

AM_CONDITIONAL([WITH_QWT], [$qwt])
AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
  ["$DISTCHECK_CONFIGURE_FLAGS --with-qwt='$QWT_PATH'"])
AC_SUBST([QWT_PATH])
])

AC_DEFUN([URBI_WITH_QWT_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_QWT])

   case "$qwt" in
    '' | 'false')
     AC_MSG_ERROR(
      [Failed to find qwt. Please install it or check config.log])
    ;;
   esac
])


## Local Variables:
## mode: autoconf
## End:
