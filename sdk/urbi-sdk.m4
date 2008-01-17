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
UMAKE_COMMON=$srcdir/libport/sdk/umake-common
AC_CONFIG_FILES([libport/sdk/umake],      [chmod +x libport/sdk/umake])
AC_CONFIG_FILES([libport/sdk/uinstall],   [chmod +x libport/sdk/uinstall])
AC_CONFIG_FILES([libport/sdk/umake-link], [chmod +x libport/sdk/umake-link])
AC_CONFIG_FILES([libport/sdk/wrapper.sh], [chmod +x libport/sdk/wrapper.sh])
AC_CONFIG_FILES([libport/sdk/Makefile])
AC_CONFIG_HEADERS([libport/sdk/config.h])
AC_CONFIG_FILES([libport/sdk/param.mk],
		[perl -w ${srcdir}/libport/sdk/eval_makefile.pl])
AC_SUBST([SDK_CFLAGS])
AC_SUBST([SDK_CXXFLAGS])
AC_SUBST([SDK_LDFLAGS])
AC_SUBST([SDK_LIBS])
])

## Local Variables:
## mode: autoconf
## End:
