# URBI_CHECK_LOCKSYSTEM_DEPENDANCIES
# ----------------
# Macro to check the dependancies of the locksystem module

AC_DEFUN([URBI_CHECK_LOCKSYSTEM_DEPENDANCIES],
[

URBI_WITH_OPENSSL_REQUIRED

AC_CHECK_PROG([COBF],[cobf],[true],[false])

if test x$COBF != xtrue; then
   AC_MSG_ERROR([
Could not find 'cobf' program necessary to obfuscate lock-system library.
Please download and install it from 'http://home.arcor.de/bernhard.baier/cobf/'.])
fi

])

# URBI_CONFIGURE_LOCKSYSTEM
# ----------------
# Macro to check the dependancies of the locksystem module

AC_DEFUN([URBI_CONFIGURE_LOCKSYSTEM],
[
URBI_CHECK_LOCKSYSTEM_DEPENDANCIES

LOCK_SYSTEM_LDFLAGS=
case "$host_os" in
     *mingw*|*cygwin*)
	LOCKSYSTEM_LDFLAGS="$LOCK_SYSTEM_LDFLAGS -lIphlpapi"
	;;
     *darwin*)
	LOCKSYSTEM_LDFLAGS="$LOCK_SYSTEM_LDFLAGS -framework CoreFoundation -framework IOKit"
	;;
     *)
	;;
esac

AC_SUBST([LOCKSYSTEM_LDFLAGS])

AC_CONFIG_SUBDIRS([lock-system])
])

# URBI_WITH_LOCKSYSTEM
# ----------------
# Macro to add to your configure when adding lock-system in
# an external.

AC_DEFUN([URBI_WITH_LOCKSYSTEM],
[
AC_ARG_ENABLE([locksystem],
             [AC_HELP_STRING([--disable-locksystem],
                             [disable softare locking system (enabled by default)])],
             [], [enable_locksystem=yes])

AM_CONDITIONAL([ENABLE_LOCKSYSTEM], [test x$enable_locksystem = xyes])

if test $enable_locksystem = yes; then
   URBI_CONFIGURE_LOCKSYSTEM
fi

])

## Local Variables:
## mode: autoconf
## End:
