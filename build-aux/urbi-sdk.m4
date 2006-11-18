m4_pattern_forbid([^ACX_])dnl
m4_pattern_forbid([^URBI_])dnl

AC_PREREQ([2.60])

# URBI_SDK
# --------
AC_DEFUN([URBI_SDK],
[AC_CONFIG_FILES([sdk/umake:build-aux/umake.in], [chmod +x sdk/umake])
AC_CONFIG_FILES([sdk/Makefile
		 sdk/sdk/param.mk])
])

## Local Variables:
## mode: autoconf
## End:
