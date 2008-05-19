# Wrappers around AC_ARG_ENABLE and AC_ARG_WITH.

# URBI_ARG_ENABLE(OPTION, HELP-STRING, RANGE, DEFAULT)
# -----------------------------------------------------
# RANGE is a globbing pattern that accepts the valid values,
# DEFAULT is the default value.
# OPTION should look like `enable-foo=bar', or `disable-bar' depending
# on the expected --help output.  Should be consistent with the DEFAULT:
# if the default is `no', use `enable-*', otherwise `disable-*'.
AC_DEFUN([URBI_ARG_ENABLE],
[m4_pushdef([AC_feature],
            [m4_bpatsubst([$1], [\(dis\|en\)able-\([^=]*\).*], [\2])])dnl
AC_ARG_ENABLE(AC_feature,
   [AC_HELP_STRING([--$1], [$2])],
   [case $enableval in
       [$3]);;
       *) AC_MSG_ERROR([--$1: bad value: $enableval]);;
    esac],
   [enable_[]m4_bpatsubst(AC_feature, [-], [_])=[$4]])
m4_popdef([AC_feature])dnl
])


## Local Variables:
## mode: autoconf
## End:
