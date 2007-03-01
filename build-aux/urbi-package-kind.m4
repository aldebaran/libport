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
[m4_bmatch(m4_defn([PACKAGE_KIND]), $@)])

## Local Variables:
## mode: autoconf
## End:
