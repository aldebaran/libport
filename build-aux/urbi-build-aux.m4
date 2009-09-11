#
# urbi-build-aux.m4: This file is part of build-aux.
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

# URBI_BUILD_AUX
# --------------
# Initialize various things for build-aux/.
AC_DEFUN([URBI_BUILD_AUX],
[# Auxiliary files.
# Currently, because libtoolize does not support traces, don't move the
# AC_CONFIG_AUX_DIR here.
# AC_CONFIG_AUX_DIR([build-aux])

# Use install-sh -C for headers.
AC_SUBST([INSTALL_HEADER], ['$(install_sh_DATA) -C'])

# Default value.
AC_SUBST([SVN], [svn])
])


## Local Variables:
## mode: autoconf
## End:
