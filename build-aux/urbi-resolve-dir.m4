dnl
# urbi-resolve-dir.m4: This file is part of build-aux.
# Copyright (C) Gostai S.A.S., 2006-2008.
dnl
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
dnl
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
dnl

# @synopsis URBI_RESOLVE_DIR_PREPARE([DIRECTORY-WITH-VARS])
#           -----------------------------------------------
# @summary Echo DIRECTORY-WITH-VARS with all the vars expanded.
#
# Variables such as $bindir that depend on $prefix and so on, are a
# pain.  Autoconf uses that style because RMS wants the user to be
# allowed to "./configure --prefix=foo" and then to "make prefix=bar",
# so bindir and the other should still depend on $prefix.  Of course
# this is doomed to failure to 99% of the cases, and the fact that
# bindir depends upon prefix is a pain for most users.
#
# The macro AC_DEFINE_DIR offers one first solution, yet it does not
# give the result as one would like to have it: directly usable in
# configure to do whatever you need it to do.
#
# Autoconf should use functions now, it's limitating and useless not
# to use them in 2007.  So I do use them here.
#
# Typical use is:
#
#   AC_SUBST([BINDIR],
#            [`URBI_RESOLVE_DIR([$bindir])`])
#
# or
#
#   bindir_full=`URBI_RESOLVE_DIR([$bindir])`
#
# if you fear $() is not portable enough.
#
# @version 2007-10-20
# @author Akim Demaille <demaille@gostai.com>
# @license GPLWithACException

m4_pattern_forbid([^URBI_])dnl

# URBI_RESOLVE_DIR_PREPARE
# ------------------------
# Define urbi_resolve_dir.
m4_defun([URBI_RESOLVE_DIR_PREPARE],
[# PATH urbi_resolve_dir(DIR)
# --------------------------
# Return the DIR with all inner variables expanded.
urbi_resolve_dir ()
{
  ac_$0_dir=$[]1
  ac_$0_res=
  ac_$0_prefix_NONE=
  ac_$0_exec_prefix_NONE=
  test "x$prefix" = xNONE &&
     ac_$0_exec_prefix_NONE=yes &&
     prefix=$ac_default_prefix
  test "x$exec_prefix" = xNONE &&
     ac_$0_exec_prefix_NONE=yes &&
     exec_prefix=$prefix
  while true
  do
    eval ac_$0_res="$ac_$0_dir"
    if test x"$ac_$0_dir" == x"$ac_$0_res"; then
      break
    fi
    ac_$0_dir=$ac_$0_res
  done
  test "$ac_$0_prefix_NONE" && prefix=NONE
  test "$ac_$0_exec_prefix_NONE" && exec_prefix=NONE
  echo "$ac_$0_res"
}
])


# PATH URBI_RESOLVE_DIR(DIR)
# --------------------------
# Return the DIR with all inner variables expanded.
AC_DEFUN([URBI_RESOLVE_DIR],
[AC_REQUIRE([URBI_RESOLVE_DIR_PREPARE])dnl
urbi_resolve_dir '$1'[]dnl
])


## Local Variables:
## mode: autoconf
## End:
