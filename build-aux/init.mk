##
## init.mk: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Initialize some common variables so that we can always use +=.
AM_CFLAGS =
AM_CPPFLAGS =
AM_CXXFLAGS =
AM_LDFLAGS =
AUTOMAKE_OPTIONS = -Wno-portability
BUILT_SOURCES =
# Visual Studio Debugger PDB Files (C++).
# http://msdn2.microsoft.com/en-us/library/yd4f8bd1.aspx.
CLEANFILES = vc80.pdb
# Files created by MSC++.
CLEANFILES += *.exe.manifest *.exe.intermediate.manifest
# Files which are not shipped should be cleaned.
CLEANFILES += $(nodist_check_SCRIPTS) $(nodist_noinst_SCRIPTS)
DISTCLEANFILES =
EXTRA_DIST =
EXTRA_HEADERS =
LIBS =
MAINTAINERCLEANFILES =

# Useful variables.
build_aux_dir = $(top_srcdir)/build-aux
