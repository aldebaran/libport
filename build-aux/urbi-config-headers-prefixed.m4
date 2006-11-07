m4_forbid_pattern([^AX_])

AC_PREREQ([2.60])

# URBI_CONFIG_HEADERS_PREFIXED(HEADER, PREFIXED-HEADER, PREFIX)
# -------------------------------------------------------------
AC_DEFUN([URBI_CONFIG_HEADERS_PREFIXED],
[AC_CONFIG_HEADERS([$1])
AX_PREFIX_CONFIG_H([$2], [$3], [$1])
AC_SUBST([PREFIXED_CONFIG_HEADER], ['$2'])
])

## Local Variables:
## mode: autoconf
## End:
