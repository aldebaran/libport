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
