#
# urbi-append-flags.m4: This file is part of build-aux.
# Copyright (C) 2006-2009, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

m4_pattern_forbid([^URBI_])
m4_pattern_forbid([^TC_])

AC_PREREQ([2.60])

AC_DEFUN([_URBI_APPEND_FLAGS],
[
# urbi_append_flags VAR FLAG...
# ----------------------------
# Append the FLAG... to $VAR, separated by spaces, unless it already
# includes it.
urbi_append_flags ()
{
  local var=$[1]
  local var
  eval "val=\$$var"
  shift
  local v
  for v
  do
    case " $val " in
      (*" $v "*) ;;
      ("  ")    val=$v;;
      (*)       val+=" $v";;
    esac
  done
  eval "$var=\$val"
}
])


# URBI_APPEND_FLAGS VAR FLAGS
# ---------------------------
# Append the FLAG... to $VAR, separated by spaces, unless it already
# includes it.
AC_DEFUN([URBI_APPEND_FLAGS],
[AC_REQUIRE([_URBI_APPEND_FLAGS])dnl
urbi_append_flags $1 $2
])

# URBI_APPEND_CPPFLAGS FLAGS
# --------------------------
AC_DEFUN([URBI_APPEND_CPPFLAGS],
[URBI_APPEND_FLAGS([CPPFLAGS], [$1])])

# URBI_APPEND_CFLAGS FLAGS
# ------------------------
AC_DEFUN([URBI_APPEND_CFLAGS],
[URBI_APPEND_FLAGS([CFLAGS], [$1])])

# URBI_APPEND_CXXFLAGS FLAGS
# --------------------------
AC_DEFUN([URBI_APPEND_CXXFLAGS],
[URBI_APPEND_FLAGS([CXXFLAGS], [$1])])

# URBI_APPEND_COMPILERFLAGS FLAGS
# -------------------------------
AC_DEFUN([URBI_APPEND_COMPILERFLAGS],
[URBI_APPEND_CFLAGS([$@])
URBI_APPEND_CXXFLAGS([$@])])


## Local Variables:
## mode: Autoconf
## End:
