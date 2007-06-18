# URBI_WITH_OPENCV
# ----------
# Look for opencv library.  Support --with-opencv=PATH. Check for
# opencv.
#
# Output variable OPENCV_PATH point to the installation location.
# Set "$opencv" to "true" or "false".
AC_DEFUN([URBI_WITH_OPENCV],
[AC_ARG_WITH([opencv],
	     [AC_HELP_STRING([--with-opencv=opencv-path],
			     [enable opencv support [/usr/local]])],
	     [], [with_opencv=yes])

case $with_opencv in
   no) opencv=false
       ;;
  yes) opencv=true
       OPENCV_PATH=/usr/local
       ;;
    *) opencv=true
       OPENCV_PATH=$with_opencv
       ;;
esac

# Checking whether Opencv library is really there.
AC_MSG_CHECKING([for opencv])
if $opencv; then
  if test -f "$OPENCV_PATH/lib/libcv.la"; then
    AC_MSG_RESULT([$OPENCV_PATH])
    opencv=true
  else
    AC_MSG_RESULT([no, opencv not found in $OPENCV_PATH])
    opencv=false
  fi
else
  AC_MSG_RESULT([no])
  opencv=false
fi

if $opencv_sdk; then
  # More tools for Opencv.
  AC_SUBST([OPENCV_CPPFLAGS], ['-I$(OPENCV_PATH)/include/opencv'])
  AC_SUBST([OPENCV_LDFLAGS],  ['-L$(OPENCV_PATH)/lib'])
  AC_SUBST([OPENCV_LDADD], ['-lcxcore -lcv -lhighgui -lcvaux -lml'])
fi

AM_CONDITIONAL([WITH_OPENCV], [$opencv])
AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
  ["$DISTCHECK_CONFIGURE_FLAGS --with-opencv='$OPENCV_PATH'"])
AC_SUBST([OPENCV_PATH])
])

AC_DEFUN([URBI_WITH_OPENCV_REQUIRED],
[
   AC_REQUIRE([URBI_WITH_OPENCV])

  case "$opencv" in
    '' | 'false')
     AC_MSG_ERROR(
      [Failed to find opencv. Please install it or check config.log])
    ;;
   esac
])

