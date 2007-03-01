# URBI_LIBPORT_INSTALLED
# ----------------------
# We use an installed libport, most probably that of the kernel we
# use.  We don't run URBI_UFLOAT since, of course, we use the same
# kind of ufloat as the kernel does.
#
# Cores use an installed libport (in kernelincludedir), *and* install it
# (in sdkincludedir).
AC_DEFUN([URBI_LIBPORT_INSTALLED],
[AC_REQUIRE([URBI_PTHREAD])

# Where we install the libport files.
URBI_PACKAGE_KIND_SWITCH(
  [kernel],     [AC_SUBST([libportdir], ['${kernelincludedir}/libport'])],
  [sdk\|core],  [AC_SUBST([libportdir], ['${sdkincludedir}/libport'])])
])

# URBI_LIBPORT
# ------------
# Invoke the macros needed by a shipped URBI_LIBPORT (i.e., a non-installed
# copy in this source tree).
AC_DEFUN([URBI_LIBPORT],
[AC_REQUIRE([URBI_UFLOAT])
AC_REQUIRE([URBI_LIBPORT_INSTALLED])

# Create libport/config.h.
URBI_PREFIX_CONFIG_H([libport/config.h], [LIBPORT])

# $(top_srcdir) to find sources, $(top_builddir) to find libport/config.h.
AC_SUBST([LIBPORT_CPPFLAGS], ['-I$(top_srcdir) -I$(top_builddir)'])
])

## Local Variables:
## mode: autoconf
## End:
