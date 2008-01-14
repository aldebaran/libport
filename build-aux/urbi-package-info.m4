dnl
dnl urbi-package-info.m4: This file is part of build-aux.
dnl Copyright (C) Gostai S.A.S., 2006-2008.
dnl
dnl This software is provided "as is" without warranty of any kind,
dnl either expressed or implied, including but not limited to the
dnl implied warranties of fitness for a particular purpose.
dnl
dnl See the LICENSE file for more information.
dnl For comments, bug reports and feedback: http://www.urbiforge.com
dnl

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
])

## Local Variables:
## mode: autoconf
## End:
