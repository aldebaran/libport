# _URBI_CHECK_LOCKSYSTEM_DEPENDENCIES
# ----------------------------------
# Macro to check the dependencies of the locksystem module.
AC_DEFUN([_URBI_CHECK_LOCKSYSTEM_DEPENDENCIES],
[URBI_WITH_OPENSSL_REQUIRED

AC_CHECK_PROG([COBF],[cobf],[true],[false])

if test x$COBF != xtrue; then
   AC_MSG_ERROR([
Could not find 'cobf' program necessary to obfuscate lock-system library.
Please download and install it from 'http://home.arcor.de/bernhard.baier/cobf/'.])
fi
])


# URBI_CONFIGURE_LOCKSYSTEM
# -------------------------
# Macro to check the dependencies of the locksystem module.
AC_DEFUN([_URBI_CONFIGURE_LOCKSYSTEM],
[_URBI_CHECK_LOCKSYSTEM_DEPENDENCIES

case $host_os in
     *mingw*|*cygwin*)
	LOCKSYSTEM_LIBS='-lIphlpapi'
	;;
     *darwin*)
	LOCKSYSTEM_LIBS='-framework CoreFoundation -framework IOKit'
	;;
esac
AC_SUBST([LOCKSYSTEM_LIBS])
AC_CONFIG_SUBDIRS([lock-system])
])


# URBI_LOCKSYSTEM_CHECK_ADDR
# ---------------------------
# Macro to add --disable-check-mac-addr
# This test of lock-system is expected to succeed only on the build-farm
# machines. You want to use this option on your workstation.
AC_DEFUN([_URBI_LOCKSYSTEM_CHECK_ADDR],
[AC_ARG_ENABLE([check-mac-addr],
             [AC_HELP_STRING([--disable-check-mac-addr],
                             [skip the test of mac address])],
             [], [enable_check_mac_addr=yes])
AM_CONDITIONAL([ENABLE_CHECK_MAC_ADDR], [test x$enable_check_mac_addr = xyes])
])

# URBI_LOCKSYSTEM_COBF
# --------------------
# Macro to add --disable-cobf
# For binary distribution we don't require to obfuscate the code.
AC_DEFUN([_URBI_LOCKSYSTEM_COBF],
[AC_ARG_ENABLE([cobf],
             [AC_HELP_STRING([--disable-cobf],
                             [disable cobf execution])],
             [], [enable_cobf=yes])
AM_CONDITIONAL([ENABLE_COBF], [test x$enable_cobf = xyes])
])

# URBI_WITH_LOCKSYSTEM
# --------------------
# Macro to add to your configure when adding lock-system in
# an external.
AC_DEFUN([URBI_WITH_LOCKSYSTEM],
[AC_ARG_ENABLE([locksystem],
	     [AC_HELP_STRING([--disable-locksystem],
			     [disable softare locking system])],
	     [], [enable_locksystem=yes])

AM_CONDITIONAL([ENABLE_LOCKSYSTEM], [test x$enable_locksystem = xyes])

_URBI_LOCKSYSTEM_CHECK_ADDR
_URBI_LOCKSYSTEM_COBF

if test $enable_locksystem = yes; then
   AC_DEFINE([USE_LOCKSYSTEM], [], [Define to use the locksystem])
   _URBI_CONFIGURE_LOCKSYSTEM
fi
])

## Local Variables:
## mode: autoconf
## End:
