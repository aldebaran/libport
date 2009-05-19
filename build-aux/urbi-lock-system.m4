#
# urbi-lock-system.m4: This file is part of build-aux.
# Copyright (C) 2006-2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

# _URBI_CHECK_LOCKSYSTEM_DEPENDENCIES
# -----------------------------------
# Macro to check the dependencies of the locksystem module.
AC_DEFUN([_URBI_CHECK_LOCKSYSTEM_DEPENDENCIES],
[URBI_WITH_OPENSSL_REQUIRED

AC_CHECK_PROG([COBF], [cobf], [true], [false])

if test x$COBF != xtrue; then
   AC_MSG_ERROR([
Could not find 'cobf' program necessary to obfuscate lock-system library.
Please download and install it from 'http://home.arcor.de/bernhard.baier/cobf/'.])
fi
])


# _URBI_CONFIGURE_LOCKSYSTEM
# --------------------------
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
SDK_LIBS="$SDK_LIBS $LOCKSYSTEM_LIBS"
AC_CONFIG_SUBDIRS([lock-system])
])


# _URBI_LOCKSYSTEM_CHECK_ADDR
# ----------------------------
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

# _URBI_LOCKSYSTEM_COBF
# ---------------------
# Macro to add --disable-cobf
# For binary distribution we don't require to obfuscate the code.
AC_DEFUN([_URBI_LOCKSYSTEM_COBF],
[AC_ARG_ENABLE([cobf],
	       [AC_HELP_STRING([--disable-cobf],
			       [disable cobf execution])],
	       [], [enable_cobf=yes])
AM_CONDITIONAL([ENABLE_COBF], [test x$enable_cobf = xyes])
case $enable_cobf in
  no)
    AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
	     ["$DISTCHECK_CONFIGURE_FLAGS '--disable-cobf'"])
    ;;
esac
])

# _URBI_LOCKSYSTEM_QT
# -------------------
# Macro to add --enable-qt-locksystem
# We provide a Qt widget that allow the user to select its license key file.
AC_DEFUN([_URBI_LOCKSYSTEM_QT],
[AC_ARG_ENABLE([qt-locksystem],
	       [AC_HELP_STRING([--enable-qt-locksystem],
			       [enable Qt widget for lock system])],
	       [enable_qt_locksystem=yes], [])
AM_CONDITIONAL([ENABLE_QT_LOCKSYSTEM], [test x$enable_qt_locksystem = xyes])
# Do we need to compile the Qt widget?
if test x$enable_qt_locksystem = xyes; then
   AT_WITH_QT
   AT_REQUIRE_QT_VERSION([4.3.2])
fi
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
_URBI_LOCKSYSTEM_QT

case $enable_locksystem in
  yes)
    AC_DEFINE([USE_LOCKSYSTEM], [], [Define to use the locksystem])
    _URBI_CONFIGURE_LOCKSYSTEM
    ;;
  no)
    AC_SUBST([DISTCHECK_CONFIGURE_FLAGS],
	     ["$DISTCHECK_CONFIGURE_FLAGS '--disable-locksystem'"])
    ;;
esac
])

## Local Variables:
## mode: autoconf
## End:
