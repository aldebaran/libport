#
# urbi-resolve-dir.m4: This file is part of build-aux.
# Copyright (C) 2006-2009, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

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

# Define urbi_resolve_dir.
m4_divert_once([M4SH-INIT],
[[# PATH urbi_resolve_dir(DIR)
# --------------------------
# Return the DIR with all inner variables expanded.
urbi_resolve_dir ()
{
  local dir="$1"
  local res=
  local prefix_NONE=
  local exec_prefix_NONE=
  test "x$prefix" = xNONE &&
     exec_prefix_NONE=yes &&
     prefix=$ac_default_prefix
  test "x$exec_prefix" = xNONE &&
     exec_prefix_NONE=yes &&
     exec_prefix=$prefix
  while true
  do
    eval res="$dir"
    if test x"$dir" == x"$res"; then
      break
    fi
    dir=$res
  done
  test "$prefix_NONE" && prefix=NONE
  test "$exec_prefix_NONE" && exec_prefix=NONE
  echo "$res"
}
]])


# PATH URBI_RESOLVE_DIR(DIR)
# --------------------------
# Return the DIR with all inner variables expanded.
AC_DEFUN([URBI_RESOLVE_DIR],
[urbi_resolve_dir '$1'[]dnl
])


## Local Variables:
## mode: autoconf
## End:
