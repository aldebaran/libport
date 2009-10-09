#
# urbi-distcheck-configure-flags.m4: This file is part of build-aux.
# Copyright (C) 2009 Gostai S.A.S.
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

# URBI_DISTCHECK_CONFIGURE_FLAGS
# ------------------------------
# Contrary to the usual use of the Autotools, we do want to use the
# same compilation tool chain in distcheck.  Enforce this, skipping
# the variables we don't want to keep.
# Warning: Beware that AC_BOOST can change DISTCHECK_CONFIGURE_FLAGS.
AC_DEFUN([URBI_DISTCHECK_CONFIGURE_FLAGS],
[# Functions provide local variables, including $[@].
distcheck_configure_flags ()
{
  local i
  for i
  do
    # Skip quotes.
    case $i in
      (--prefix=*|-C|--cache-file=*|--srcdir=*) ;;
      (*) # Remember that this will go into the Makefile, so we
          # must escape properly according to Makefile rules.  In
          # particular, escape $ as $$.
          #
          # FIXME: Currently we are unsafe wrt shell metacharacters.
          ac_cv_distcheck_configure_flags="$ac_cv_distcheck_configure_flags '${i//\$/\$\$}'";;
    esac
  done
}

AC_CACHE_CHECK([for configure arguments for distcheck],
                [ac_cv_distcheck_configure_flags],
[ac_cv_distcheck_configure_flags=
eval distcheck_configure_flags $ac_configure_args
])
AC_SUBST([DISTCHECK_CONFIGURE_FLAGS], [$ac_cv_distcheck_configure_flags])
])


## Local Variables:
## mode: autoconf
## End:
