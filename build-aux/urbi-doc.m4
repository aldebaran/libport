#
# urbi-doc.m4: This file is part of build-aux.
# Copyright (C) 2006-2009, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

m4_pattern_forbid([^URBI_])dnl

AC_PREREQ([2.60])

# URBI_ARG_ENABLE_DOC
# -------------------
AC_DEFUN([URBI_ARG_ENABLE_DOC],
[AC_ARG_ENABLE([doc],
   AS_HELP_STRING([--enable-doc],
                  [Turn on documentation (default is NO)]),
     [case $enable_doc in
       (yes|true|"")
            doc_output=doc
            doc_install=doc-install
            doc_uninstall=doc-uninstall;;
       (no|false)
            doc_output=
            doc_install=
            doc_uninstall=;;
       (*) AC_MSG_ERROR([bad value $enable_doc for --enable-doc]) ;;
     esac],
  [doc_output=
   doc_install=
   doc_uninstall=])
#AM_CONDITIONAL([COND_DOC_OUTPUT], [test x$doc_output = xtrue])
AC_SUBST([COND_DOC_OUTPUT], [$doc_output])
AC_SUBST([COND_DOC_INSTALL], [$doc_install])
AC_SUBST([COND_DOC_UNINSTALL], [$doc_uninstall])
])


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

URBI_ARG_ENABLE_DOC
])


## Local Variables:
## mode: autoconf
## End:
