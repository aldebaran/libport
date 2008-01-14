dnl
dnl enable-emacs-output.m4: This file is part of build-aux.
dnl Copyright (C) Gostai S.A.S., 2006-2008.
dnl
dnl This software is provided "as is" without warranty of any kind,
dnl either expressed or implied, including but not limited to the
dnl implied warranties of fitness for a particular purpose.
dnl
dnl See the LICENSE file for more information.
dnl For comments, bug reports and feedback: http://www.urbiforge.com
dnl

# ENABLE_EMACS_OUTPUT
# ---
# Enable Emacs compatible output.
#
# Output variable ENABLE_EMACS_OUTPUT may be used in Makefile.am. Typically,
# eamcs-qtestlib.mk uses it to enable filtering of the QTestLib output in order
# to make it compatible with Emacs compile mode.

AC_DEFUN([ENABLE_EMACS_OUTPUT],
[AC_ARG_ENABLE([emacs-output],
   AS_HELP_STRING([--enable-emacs-output],
                  [Turn on Emacs compatible output (default is NO)]),
     [case "${enableval}" in
       yes) emacs_output=true ;;
       no)  emacs_output=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-emacs-output]) ;;
     esac],
  [emacs_output=false])
AM_CONDITIONAL([COND_EMACS_OUTPUT], [test x$emacs_output = xtrue])
AC_SUBST([COND_EMACS_OUTPUT])
])

## Local Variables:
## mode: autoconf
## End:
