dnl
dnl urbi-config-headers-prefixed.m4: This file is part of build-aux.
dnl Copyright (C) Gostai S.A.S., 2006-2008.
dnl
dnl This software is provided "as is" without warranty of any kind,
dnl either expressed or implied, including but not limited to the
dnl implied warranties of fitness for a particular purpose.
dnl
dnl See the LICENSE file for more information.
dnl For comments, bug reports and feedback: http://www.urbiforge.com
dnl

m4_forbid_pattern([^AX_])

AC_PREREQ([2.60])

# URBI_PREFIX_CONFIG_H([PREFIXED-HEADER], [PREFIX], [ORIG-HEADER])
# ----------------------------------------------------------------
# See the AX_PREFIX_CONFIG_H documentation.
AC_DEFUN([URBI_PREFIX_CONFIG_H],
[AX_PREFIX_CONFIG_H($@)
AC_SUBST([PREFIXED_CONFIG_HEADER], ['$1'])
])

# URBI_CONFIG_HEADERS_PREFIXED(HEADER, PREFIXED-HEADER, PREFIX)
# -------------------------------------------------------------
AC_DEFUN([URBI_CONFIG_HEADERS_PREFIXED],
[AC_CONFIG_HEADERS([$1])
URBI_PREFIX_CONFIG_H([$2], [$3], [$1]))
])

## Local Variables:
## mode: autoconf
## End:
