# URBI_OPENR
# ----------
# Look for the OpenR SDK.  Support --with-openr=PATH.  Check the compiler
# is there.
#
# Output variable OPEN_R_SDK point to the installation location.
# Set "$openr" to "true" or "false".
AC_DEFUN([URBI_OPENR],
[AC_ARG_WITH([openr],
	     [AC_HELP_STRING([--with-openr=sdk-path],
			     [Turn on OpenR client [/usr/local/OPEN_R_SDK]])],
	     [], [with_openr=/usr/local/OPEN_R_SDK])

case $with_openr in
   no) openr=false
       ;;
    *) openr=true
       OPENR_SDK_PATH=$with_openr
       ;;
esac

# Checking whether OpenR SDK is really there.
AC_MSG_CHECKING([for OpenR SDK])
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

if $openr; then
  # More tools for OpenR.
  AC_SUBST([MKBIN],       ['$(OPEN_R_SDK)/OPEN_R/bin/mkbin'])
  AC_SUBST([MKBIN_FLAGS], ['-p $(OPEN_R_SDK)'])
  AC_SUBST([STUBGEN],     ['$(OPEN_R_SDK)/OPEN_R/bin/stubgen2'])
  AC_SUBST([OPEN_R_CPPFLAGS],
  ['-I$(OPEN_R_SDK)/OPEN_R/include/R4000 \
    -I$(OPEN_R_SDK)/OPEN_R/include/MCOOP \
    -I$(OPEN_R_SDK)/OPEN_R/include'])
fi

AM_CONDITIONAL([OPENR], [$openr])
AC_SUBST([OPEN_R_SDK])
])


## Local Variables:
## mode: autoconf
## End:
