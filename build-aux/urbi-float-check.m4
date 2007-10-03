# URBI_FLOAT_CHECK
# ----------------
# Look for various standard functions related to float that might not be
# portable (such as round)
AC_DEFUN([URBI_FLOAT_CHECK],
[
  AC_CHECK_FUNCS([round])
])

## Local Variables:
## mode: autoconf
## End:
