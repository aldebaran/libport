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
AC_SUBST([install_sh_c_DATA], ['$(install_sh)-c -c -m 644'])
AC_SUBST([INSTALL_HEADER], ['$(install_sh_c_DATA) -C'])
])


## Local Variables:
## mode: autoconf
## End:
