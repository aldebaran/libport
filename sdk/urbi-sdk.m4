m4_pattern_forbid([^URBI_])dnl

AC_PREREQ([2.60])

# URBI_SDK
# --------
# Instantiate the files required by SDKs.  Instantiate param.mk.
# The various flags that are needed by the SDK should be passed
# here.  This is painful: it must be kept up to date in two different
# places: where you detect these flags, and here.  But I have nothing
# better for the moment.  Hopefully flags such as LOCKSYSTEM_LIBS will
# be handled by libtool some day.
AC_DEFUN([URBI_SDK],
[AC_SUBST([BINDIR],
	  [$(URBI_RESOLVE_DIR([$bindir]))])
AC_SUBST_FILE([UMAKE_COMMON])
UMAKE_COMMON=$srcdir/sdk/umake-common
AC_CONFIG_FILES([sdk/umake],      [chmod +x sdk/umake])
AC_CONFIG_FILES([sdk/uinstall],   [chmod +x sdk/uinstall])
AC_CONFIG_FILES([sdk/umake-link], [chmod +x sdk/umake-link])
AC_CONFIG_FILES([sdk/wrapper.sh], [chmod +x sdk/wrapper.sh])
AC_CONFIG_HEADERS([sdk/config.h])
AC_CONFIG_FILES([sdk/param.mk],
		[perl -w ${srcdir}/sdk/eval_makefile.pl])
AC_SUBST([SDK_CFLAGS])
AC_SUBST([SDK_CXXFLAGS])
AC_SUBST([SDK_LDFLAGS])
AC_SUBST([SDK_LIBS])

# shrext_cmds is set by libtool, ensure AC_PROG_LIBTOOL is called
# before URBI_SDK.  It may either be value, or a command in
# backquotes: it is a command, not a value.  See the comment about OSX
# below to see why.
#
# Yet we want an effective value: decode it.
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
AC_SUBST([SHLIBEXT])
])

## Local Variables:
## mode: autoconf
## End:
