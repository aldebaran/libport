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
