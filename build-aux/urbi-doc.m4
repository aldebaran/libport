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
            DOC_OUTPUT=doc
            DOC_INSTALL=doc-install
            DOC_UNINSTALL=doc-uninstall;;
       (no|false)
            DOC_OUTPUT=
            DOC_INSTALL=
            DOC_UNINSTALL=;;
       (*) AC_MSG_ERROR([bad value $enable_doc for --enable-doc]) ;;
     esac],
  [DOC_OUTPUT=
   DOC_INSTALL=
   DOC_UNINSTALL=])
#AM_CONDITIONAL([COND_DOC_OUTPUT], [test x$doc_output = xtrue])
AC_SUBST([DOC_OUTPUT])
AC_SUBST([DOC_INSTALL])
AC_SUBST([DOC_UNINSTALL])
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
AC_SUBST([DOXYGEN], [doxygen])
AC_CONFIG_FILES([doc/Makefile
		 doc/Doxyfile:build-aux/Doxyfile.in])

URBI_ARG_ENABLE_DOC
])


## Local Variables:
## mode: autoconf
## End:
