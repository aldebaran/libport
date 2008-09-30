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
AUTOMAKE_OPTIONS = -Wall -Wno-portability 1.10 nostdinc
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
SUFFIXES =
# Automake still does not honor AM_LDADD.
LDADD = $(AM_LDADD)

nodist_bin_SCRIPTS =
dist_bin_SCRIPTS =

bin_PROGRAMS =

# Useful variables.
build_aux_dir = $(top_srcdir)/build-aux

# Sometimes it is really convenient to see the output of the
# preprocessor.  But it's a pain to run the command by hand.
SUFFIXES += .i .ii
.c.i:
	$(COMPILE) -E -o $@ $<

.cc.ii:
	$(CXXCOMPILE) -E -o $@ $<

CLEANFILES += *.i *.ii

# Sometimes, the remaining # lines are a nuisance: the error is not
# reported against the preprocessed file.  And then we also want to
# compile these files, that's why I used extensions that can be
# directly compiled (*.c and *.cc).
SUFFIXES += .i.c .ii.cc
.i.i.c:
	grep -v '#' $< | tr -s '\n' '\n' >$@.tmp
	mv $@.tmp $@

.ii.ii.cc:
	grep -v '#' $< | tr -s '\n' '\n' >$@.tmp
	mv $@.tmp $@

CLEANFILES += *.i.c *.ii.cc
