# LIBURBI
# ----------
# Look for LibUrbi.  Support --with-liburbi=PATH. Check the
# liburbi library.
#
# Output variable LIBURBI_PATH point to the installation location.
# Set "$liburbi" to "true" or "false".
AC_DEFUN([LIBURBI],
[AC_ARG_WITH([liburbi],
	     [AC_HELP_STRING([--with-liburbi=liburbi-path],
			     [enable liburbi support [/usr/local]])],
	     [], [with_liburbi=yes])

case $with_liburbi in
   no) liburbi=false
       ;;
  yes) liburbi=true
       LIBURBI_PATH=/usr/local
       ;;
    *) liburbi=true
       LIBURBI_PATH=$with_liburbi
       ;;
esac

# Checking whether LibUrbi is really there.
AC_MSG_CHECKING([for LibUrbi])
if $liburbi; then
  if test -f $LIBURBI_PATH/lib/liburbi.la; then
    AC_MSG_RESULT([$LIBURBI_PATH])
    liburbi=true
  else
    AC_MSG_RESULT([no, liburbi no found in $LIBURBI_PATH])
    liburbi=false
  fi
else
  AC_MSG_RESULT([no])
  liburbi=false
fi

if $liburbi; then
  # More tools for LibUrbi.
  AC_SUBST([LIBURBI_CPPFLAGS], ['-I$(LIBURBI_PATH)/include'])
  AC_SUBST([LIBURBI_LDFLAGS],  ['-L$(LIBURBI_PATH)/lib'])
  AC_SUBST([LIBURBI_LIBS],  ['$(LIBURBI_PATH)/lib/liburbi.la'])
fi

AM_CONDITIONAL([LIBURBI], [$liburbi])
AC_SUBST([LIBURBI_PATH])
])

AC_DEFUN([LIBURBI_REQUIRED],
[
   AC_REQUIRE([LIBURBI])

   if test -z $liburbi -o $liburbi = false; then
      AC_MSG_ERROR([Failed to find LibUrbi. Please install it or check config.log])
   fi
])


## Local Variables:
## mode: autoconf
## End:
