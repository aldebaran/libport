# URBI_LIBPORT
# ------------
# Invoke the macros needed by URBI_LIBPORT.
AC_DEFUN([URBI_LIBPORT],
[AC_REQUIRE([URBI_UFLOAT])
AC_REQUIRE([URBI_PTHREAD])

# Create libport/config.h.
URBI_PREFIX_CONFIG_H([libport/config.h], [LIBPORT])

# $(top_srcdir) to find sources, $(top_builddir) to find libport/config.h.
AC_SUBST([LIBPORT_CPPFLAGS], ['-I$(top_srcdir) -I$(top_builddir)'])
])


## Local Variables:
## mode: autoconf
## End:
