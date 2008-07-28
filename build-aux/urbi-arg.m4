# Wrappers around AC_ARG_ENABLE and AC_ARG_WITH.

# _URBI_ARG_OPTION_BRE
# --------------------
# A BRE that match valid OPTION for _URB_ARG.
m4_define([_URBI_ARG_OPTION_BRE],
          [^\(disable\|enable\|with\|without\)-\([^=]*\).*$])


# _URBI_ARG(KIND, OPTION, HELP-STRING, RANGE, DEFAULT, [MORE-HELP])
# -----------------------------------------------------------------
# KIND is either enable (AC_ARG_ENABLE) or with (AC_ARG_DISABLE).
#
# RANGE is an ERE pattern (that will be anchored on both ends)
#   that accepts the valid values.
#
# DEFAULT is the default value.
#
# OPTION should look like `enable-foo=bar', or `disable-bar' depending
# on the expected --help output.  Should be consistent with the DEFAULT:
# if the default is `no', use `enable-*', otherwise `disable-*'.
#
# MORE-HELP is output as if in the output, while HELP-STRING is
# formatted via AC_HELP_STRING.
AC_DEFUN([_URBI_ARG],
[AC_REQUIRE([AC_PROG_EGREP])dnl
m4_if([m4_bregexp([$2], m4_defn([_URBI_ARG_OPTION_BRE]))],
      [-1], [m4_fatal([$0: invalid OPTION: --$2])])dnl
m4_pushdef([AC_feature],
           [m4_bpatsubst([$2],
                         m4_defn([_URBI_ARG_OPTION_BRE]),
                         [\2])])dnl
m4_indir(m4_case([$1],
                 [enable], [[AC_ARG_ENABLE]],
                 [with], [[AC_ARG_WITH]],
                 [m4_fatal([$0: invalid kind: $1])]),
         AC_feature,
         [AC_HELP_STRING([--$2], [$3 [$5]])[]$6],
   [if echo "$$1val" | $EGREP '^($4)$' >/dev/null; then :; else
     AC_MSG_ERROR([--$2: bad value: $$1val])
    fi],
   [$1_[]m4_bpatsubst(AC_feature, [-], [_])=[$5]])
m4_popdef([AC_feature])dnl
])


# URBI_ARG_ENABLE(OPTION, HELP-STRING, RANGE, DEFAULT, [MORE-HELP])
# -----------------------------------------------------------------
AC_DEFUN([URBI_ARG_ENABLE],
[_URBI_ARG([enable], [$1], [$2], [$3], [$4], [$5])])


# URBI_ARGLIST_ENABLE(OPTION, HELP-STRING, RANGE, DEFAULT, [MORE-HELP])
# ---------------------------------------------------------------------
# Same as URBI_ARG_ENABLE, but accept a coma-seperated list of args.
AC_DEFUN([URBI_ARGLIST_ENABLE],
[URBI_ARG_ENABLE([$1], [$2], [($3)(,($3))*], [$4], [$5])])


# URBI_ARG_WITH(OPTION, HELP-STRING, RANGE, DEFAULT, [MORE-HELP])
# ---------------------------------------------------------------
AC_DEFUN([URBI_ARG_WITH],
[_URBI_ARG([with], [$1], [$2], [$3], [$4], [$5])])


## Local Variables:
## mode: autoconf
## End:
