# Wrappers around AC_ARG_ENABLE and AC_ARG_WITH.

# URBI_ARG_ENABLE(OPTION, HELP-STRING, RANGE, DEFAULT)
# -----------------------------------------------------
# RANGE is an ERE pattern (that will be anchored on both ends)
#   that accepts the valid values.
# DEFAULT is the default value.
# OPTION should look like `enable-foo=bar', or `disable-bar' depending
# on the expected --help output.  Should be consistent with the DEFAULT:
# if the default is `no', use `enable-*', otherwise `disable-*'.
AC_DEFUN([URBI_ARG_ENABLE],
[AC_REQUIRE([AC_PROG_EGREP])dnl
m4_pushdef([AC_feature],
            [m4_bpatsubst([$1], [\(dis\|en\)able-\([^=]*\).*], [\2])])dnl
AC_ARG_ENABLE(AC_feature,
   [AC_HELP_STRING([--$1], [$2])],
   [if echo "$enableval" | $EGREP '^($3)$' >/dev/null; then :; else
     AC_MSG_ERROR([--$1: bad value: $enableval])
    fi],
   [enable_[]m4_bpatsubst(AC_feature, [-], [_])=[$4]])
m4_popdef([AC_feature])dnl
])


# URBI_ARGLIST_ENABLE(OPTION, HELP-STRING, RANGE, DEFAULT)
# --------------------------------------------------------
# Same as URBI_ARG_ENABLE, but accept a coma-seperated list of args.
AC_DEFUN([URBI_ARGLIST_ENABLE],
[URBI_ARG_ENABLE([$1], [$2], [($3)(,($3))*], [$4])
])


## Local Variables:
## mode: autoconf
## End:
