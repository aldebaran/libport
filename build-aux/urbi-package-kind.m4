#
# urbi-package-kind.m4: This file is part of build-aux.
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

AC_PREREQ([2.60])

# URBI_PACKAGE_KIND(KIND)
# -----------------------
# depending on the environment using this macro, KIND should be:
#  - core
#    engines and cores.
#  - sdk
#    liburbi-cpp and other sdks.
#  - kernel
#    kernels.
#
# Used by URBI_DIRS and others.
#
# Defines (and AC_SUBST's) PACKAGE_KIND.
AC_DEFUN([URBI_PACKAGE_KIND],
[m4_bmatch([$1],
	   [core\|kernel\|sdk], [],
	   [m4_fatal([$0: invalid kind: $1])])
m4_define([PACKAGE_KIND], [$1])
AC_SUBST([PACKAGE_KIND], [$1])
])

# URBI_PACKAGE_KIND_SWITCH(PATTERN1, EXPANSION1,
#                          ...
#                          [DEFAULT EXPANSION])
# ----------------------------------------------
m4_define([URBI_PACKAGE_KIND_SWITCH],
[m4_after([$0], [URBI_PACKAGE_KIND])dnl
m4_bmatch(m4_defn([PACKAGE_KIND]), $@)dnl
])

## Local Variables:
## mode: autoconf
## End:
