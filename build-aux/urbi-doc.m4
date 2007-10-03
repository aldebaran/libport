m4_pattern_forbid([^URBI_])dnl

AC_PREREQ([2.60])

# URBI_DOC
# --------
AC_DEFUN([URBI_DOC],
[
## --------------- ##
## Documentation.  ##
## --------------- ##

URBI_ARG_PROGS([doxygen], [the Doxygen documentation generation program])
# Name of the directory where the Doxygen documentation is created.
AC_SUBST([DOCDIR], [srcdoc])

AC_CONFIG_FILES([doc/Makefile
		 doc/Doxyfile:build-aux/Doxyfile.in])

ENABLE_DOC
])

AC_DEFUN([ENABLE_DOC],
[AC_ARG_ENABLE([doc],
   AS_HELP_STRING([--enable-doc],
                  [Turn on documentation (default is NO)]),
     [], [enable_doc=no])
AM_CONDITIONAL([ENABLE_DOC], [test x$enable_doc != xno])
])

## Local Variables:
## mode: autoconf
## End:
