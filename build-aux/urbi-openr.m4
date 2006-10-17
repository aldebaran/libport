# URBI_OPENR
# ----------
# Look for the OpenR SDK.  Support --with-openr=PATH.  Check the compiler
# is there.
# Output variable OPEN_R_SDK point to the installation location.
AC_DEFUN([URBI_OPENR],
[AC_ARG_WITH([openr],
	    [AC_HELP_STRING([--with-openr=sdk-path],
			    [Turn on OPENR client [/usr/local/OPEN_R_SDK]])],
	    [], [with_openr=yes])

case $with_openr in
  yes) openr=true
       OPEN_R_SDK=/usr/local/OPEN_R_SDK
       ;;
   no) openr=false
       ;;
    *) openr=true
       OPENR_SDK_PATH=$with_openr
       ;;
esac


# checking if openr sdk is realy there
AC_MSG_CHECKING([for openr SDK])
if $openr; then
  if test -f $OPEN_R_SDK/bin/mipsel-linux-c++; then
    AC_MSG_RESULT([$OPEN_R_SDK])
  else
    AC_MSG_RESULT([no, mipsel c++ compiler not found in $OPEN_R_SDK/bin])
    openr=false
  fi
else
  AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([OPENR], [$openr])
AC_SUBST([OPEN_R_SDK])
])


## Local Variables:
## mode: autoconf
## End:
