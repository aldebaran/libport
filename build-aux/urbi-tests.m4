AC_PREREQ([2.60])

# URBI_CONFIG_WRAPPER COMPONENT-NAME
# ----------------------------------
# Create a testing wrapper for the COMPONENT-NAME.
m4_define([URBI_CONFIG_WRAPPER],
[AC_CONFIG_FILES([tests/$1:tests/wrapper.in], [chmod a+x tests/$1])
])

# URBI_TESTS
# ----------
# Set up the urbi test suite (in tests/).
AC_DEFUN([URBI_TESTS],
[# Prepare the umake wrappers.
URBI_CONFIG_WRAPPER([umake])
URBI_CONFIG_WRAPPER([umake-engine])
URBI_CONFIG_WRAPPER([umake-lib])

# Prepare the Makefile.
AC_CONFIG_FILES([tests/Makefile])
])

## Local Variables:
## mode: autoconf
## End:
