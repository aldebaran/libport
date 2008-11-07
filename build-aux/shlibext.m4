m4_pattern_forbid([^URBI_])dnl

AC_PREREQ([2.60])

# URBI_SHLIBEXT
# -------------
# shrext_cmds is set by libtool, ensure AC_PROG_LIBTOOL is called
# before URBI_SDK.  It may either be value, or a command in
# backquotes: it is a command, not a value.  See the comment about OSX
# below to see why.
#
# Yet we want an effective value: decode it.
AC_DEFUN([URBI_SHLIBEXT],
[AC_MSG_CHECKING([for the shared library extension])
case $shrext_cmds in
  ('') # AC_PROG_LIBTOOL did shit.
     AC_MSG_ERROR([\$shrext_cmds not defined: was AC@&t@_PROG_LIBTOOL invoked?]);;
  (.*)
     # A regular extensions.
     SHLIBEXT=$shrext_cmds;;
  (*test*module*so*dylib*)
     # On Mac OSX, dlopened modules are *.so, and shlib are *.dylib,
     # and libtools sets
     #   shrext_cmds='`test .$module = .yes && echo .so || echo .dylib`'
     # that depends on $module which is a runtime value.
     # Currently, we use only *.dylib on OSX.
     SHLIBEXT=.dylib;;
  (*)
     AC_MSG_WARN([unexpected \$shlibext_cmds: $shlibext_cmds])
     SHLIBEXT=$shrext_cmds;;
esac
AC_MSG_RESULT([$SHLIBEXT])
AC_SUBST([SHLIBEXT])
])

## Local Variables:
## mode: autoconf
## End:
