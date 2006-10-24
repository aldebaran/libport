
# URBI_UFLOAT
# -----------
AC_DEFUN([URBI_UFLOAT],
[# Whether we use special ufloats or not.
AC_ARG_ENABLE([ufloat],
	      [AC_HELP_STRING([--enable-ufloat],
			      [use ufloats instead of doubles])],
	      [], [enable_ufloat=no])
AC_MSG_CHECKING([whether using ufloats])
AC_MSG_RESULT([$enable_ufloat])
AM_CONDITIONAL([UFLOAT], [test x"$enable_ufloat" = xyes])
if test x"$enable_ufloat" = xyes; then
  AC_DEFINE([HAVE_LIBPORT_UFLOAT_HH], [1],
	    [Define to 1 if you want to use ufloat.hh instead of plain doubles])
fi
])


## Local Variables:
## mode: autoconf
## End:
