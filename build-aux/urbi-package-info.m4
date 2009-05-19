#
# urbi-package-info.m4: This file is part of build-aux.
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

AC_PREREQ([2.60])

# URBI_PACKAGE_COPYRIGHT(HOLDER, YEARS)
# -------------------------------------
# This is used by libport/package-info.*.
AC_DEFUN([URBI_PACKAGE_COPYRIGHT],
[m4_define([AC_PACKAGE_COPYRIGHT_HOLDER], [$1])
AC_SUBST([PACKAGE_COPYRIGHT_HOLDER], ['$1'])
AC_DEFINE_UNQUOTED([PACKAGE_COPYRIGHT_HOLDER], ["$PACKAGE_COPYRIGHT_HOLDER"],
		   [Define to the copyright holder of this package.])

m4_define([AC_PACKAGE_COPYRIGHT_YEARS], [$2])
AC_SUBST([PACKAGE_COPYRIGHT_YEARS], ['$2'])
AC_DEFINE_UNQUOTED([PACKAGE_COPYRIGHT_YEARS], ["$PACKAGE_COPYRIGHT_YEARS"],
		   [Define to the copyright years of this package.])

m4_define([AC_PACKAGE_COPYRIGHT], [Copyright (C) $2, $1])
m4_bmatch([$1], [[^!?.]$],
          [m4_append([AC_PACKAGE_COPYRIGHT], [.])])
AC_SUBST([PACKAGE_COPYRIGHT], ['m4_defn([AC_PACKAGE_COPYRIGHT])'])
AC_DEFINE_UNQUOTED([PACKAGE_COPYRIGHT], ["$PACKAGE_COPYRIGHT"],
		   [Define to the copyright line of this package,
                    including the period.])
])

## Local Variables:
## mode: autoconf
## End:
