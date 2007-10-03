# URBI_FLOAT_CHECK
# ----------------
# Look for various standard functions related to float that might not be
# portable (such as round).
AC_DEFUN([URBI_FLOAT_CHECK],
[
  # On Aibo (mipsel-linux-g++) we don't have `round' and `trunc' so we must be
  # able to provide replacement implementations if needed.
  AC_CHECK_FUNCS([round])
  AC_CHECK_FUNCS([trunc])
])

## Local Variables:
## mode: autoconf
## End:
