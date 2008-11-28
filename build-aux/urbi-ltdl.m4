#
# urbi-tdl.m4: This file is part of build-aux.
# Copyright (C) 2008, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#

m4_pattern_forbid([^URBI_])

AC_PREREQ([2.60])

# URBI_LTDL
# ---------
# Use libltdl as shipped by libport, as a convenience library.
#
# To complete the use, one needs to add the following lines in the
# top-level Makefile:
#
# libtool: $(LIBTOOL_DEPS)
#	$(SHELL) ./config.status --recheck
#
# and use LTDL_CPPFLAGS and LTDL_LIBS where appropriate.  You may also
# use LTDL_DEPS, but beware that specifying PROG_DEPENDENCIES stops
# Automake from generating dependencies from LDADD, which is a
# nuisance.
AC_DEFUN([URBI_LTDL],
[# If you need to create a library, use libtool.
LT_PREREQ([2.2.6])
# Name the subdirectory that contains libltdl sources
LT_CONFIG_LTDL_DIR([libltdl])
LT_INIT([pic-only shared disable-static dlopen win32-dll])
# Work around a bug in Automake/Libtool, this dir should have been
# created.
$MKDIR_P libltdl/.deps .deps/libltdl
LTDL_INIT([nonrecursive])
AC_SUBST([LIBTOOL_DEPS])

AC_SUBST([LTDL_CPPFLAGS], ['$(LTDLINCL)'])
AC_SUBST([LTDL_LIBS],     ['$(LIBLTDL)'])
AC_SUBST([LTDL_DEPS],     ['$(LTDLDEPS)'])

URBI_LIBTOOL
])

## Local Variables:
## mode: autoconf
## End:
