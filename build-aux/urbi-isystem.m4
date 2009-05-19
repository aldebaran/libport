#
# urbi-prog-cxx.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
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

# URBI_ISYSTEM(CPPFLAGS-VARIABLE-NAMES)
# -------------------------------------
# In the space separated list of CPPFLAGS-VARIABLE-NAMES, replace
# -I with -isystem if applicable.
AC_DEFUN([URBI_ISYSTEM],
[for ac_var in $1
do
  AC_MSG_CHECKING([for isystem use in $ac_var])
  # If using GCC, prefer -isystem to -I for vendor supplied headers.
  case $GCC in
  yes)
    eval ac_val=\$$ac_var
    ac_val=$(echo "$ac_val" | perl -p -e 's/(?<!-)-I\s*/-isystem /g')
    eval $ac_var=\$ac_val
    ;;
  esac
  AC_MSG_RESULT([$ac_val])
done
])

## Local Variables:
## mode: autoconf
## End:
