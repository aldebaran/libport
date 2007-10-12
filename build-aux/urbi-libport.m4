# _URBI_LIBPORT_COMMON
# --------------------
# Code common to the use of an installed or shipped libport.
AC_DEFUN([_URBI_LIBPORT_COMMON],
[AC_REQUIRE([URBI_PTHREAD])dnl
AC_REQUIRE([URBI_FLOAT_CHECK])dnl
# Check for Boost headers
AX_BOOST_BASE([1.34])
# Check for Boost.Thread
if test -n "$openr" && $openr; then
  AC_MSG_NOTICE([[OpenR in use: not using Boost.Thread.]])
else
  AX_BOOST_THREAD
  if test "x$BOOST_THREAD_LIB" = "x"; then
    AC_MSG_FAILURE([[Sorry, we need the Boost.Thread library from Boost.
	    Please install from http://www.boost.org]])
  fi
fi

# Where we install the libport files.
URBI_PACKAGE_KIND_SWITCH(
  [kernel],     [AC_SUBST([libportdir], ['${kernelincludedir}/libport'])],
  [sdk\|core],  [AC_SUBST([libportdir], ['${sdkincludedir}/libport'])])
])


# URBI_LIBPORT_INSTALLED
# ----------------------
# We use an installed libport, most probably that of the kernel we
# use.  We don't run URBI_UFLOAT since, of course, we use the same
# kind of ufloat as the kernel does.
#
# Cores use an installed libport (in kernelincludedir), *and* install it
# (in sdkincludedir).
AC_DEFUN([URBI_LIBPORT_INSTALLED],
[AC_REQUIRE([_URBI_LIBPORT_COMMON])dnl
AC_AFTER([$0], [URBI_DIRS])dnl

# Check that we can find libport files.
libport_config_hh=$(URBI_RESOLVE_DIR([$kernelincludedir/libport/config.h]))
if test ! -f $libport_config_h; then
  AC_ERROR([--with-urbi-kernel: cannot find $libport_config_h])
fi
])

# URBI_LIBPORT
# ------------
# Invoke the macros needed by a shipped URBI_LIBPORT (i.e., a non-installed
# copy in this source tree).
AC_DEFUN([URBI_LIBPORT],
[AC_REQUIRE([_URBI_LIBPORT_COMMON])dnl
AC_REQUIRE([URBI_UFLOAT])dnl

# Create libport/config.h.
URBI_PREFIX_CONFIG_H([libport/config.h], [LIBPORT])

# $(top_srcdir) to find sources, $(top_builddir) to find libport/config.h.
AC_SUBST([LIBPORT_CPPFLAGS], ['-I$(top_srcdir) -I$(top_builddir)'])

AC_CONFIG_FILES([libport/Makefile])
])

## Local Variables:
## mode: autoconf
## End:
