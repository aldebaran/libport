dnl
dnl urbi-openr.m4: This file is part of build-aux.
dnl Copyright (C) Gostai S.A.S., 2006-2008.
dnl
dnl This software is provided "as is" without warranty of any kind,
dnl either expressed or implied, including but not limited to the
dnl implied warranties of fitness for a particular purpose.
dnl
dnl See the LICENSE file for more information.
dnl For comments, bug reports and feedback: http://www.urbiforge.com
dnl

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
			     [turn on OpenR client [/usr/local/OPEN_R_SDK]])],
	     [], [with_openr=no])

case $with_openr in
   no) openr=false
       ;;
  yes) openr=true
       OPEN_R_SDK=/usr/local/OPEN_R_SDK
       ;;
    *) openr=true
       OPEN_R_SDK=$with_openr
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
  if test x"$cross_compiling" = xno; then
    AC_MSG_NOTICE([**** WARNING **** Using OpenR, but not a cross compiler])
  fi

  # More tools for OpenR.
  AC_SUBST([MKBIN],       ['$(OPEN_R_SDK)/OPEN_R/bin/mkbin'])
  AC_SUBST([MKBIN_FLAGS], ['-p $(OPEN_R_SDK)'])
  AC_SUBST([STUBGEN],     ['$(OPEN_R_SDK)/OPEN_R/bin/stubgen2'])
  AC_SUBST([OPEN_R_CPPFLAGS],
  ['-I$(OPEN_R_SDK)/OPEN_R/include/R4000 \
    -I$(OPEN_R_SDK)/OPEN_R/include/MCOOP \
    -I$(OPEN_R_SDK)/OPEN_R/include'])
  AC_SUBST([OPEN_R_LDFLAGS],
  ['-L$(OPEN_R_SDK)/OPEN_R/lib \
    -lObjectComm -lERA201D1 -lOPENR -lInternet -lantMCOOP -lm'])

  # Make distcheck made simpler.
  AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
	   ["$DISTCHECK_CONFIGURE_FLAGS --with-openr='\$(OPEN_R_SDK)'"])

  # Disable the compilation of the programs in jpeg.
  AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
	   ["$DISTCHECK_CONFIGURE_FLAGS --disable-programs"])

  # Set up cross compilation tools.
  AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
	   ["$DISTCHECK_CONFIGURE_FLAGS\
 --host='\$(host_alias)'\
 --build='\$(build_alias)'\
 CC='\$(CC)'\
 CXX='\$(CXX)'"])
fi

AM_CONDITIONAL([OPENR], [$openr])
AC_SUBST([OPEN_R_SDK])
])


## Local Variables:
## mode: autoconf
## End:
