##
## m4sh.mk: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

# Use autom4te to create our scripts, with additions stored in
# $(top_srcdir)build-aux.  Define m4sh_scripts.

## ------------- ##
## The scripts.  ##
## ------------- ##

edit =									  \
 sed									  \
  -e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g'				  \
  -e 's|@PACKAGE_TARNAME[@]|$(PACKAGE_TARNAME)|g'			  \
  -e 's|@PERL[@]|$(PERL)|g'						  \
  -e 's|@SHELL[@]|$(SHELL)|g'						  \
  -e 's|@VERSION[@]|$(VERSION)|g'					  \
  -e 's|@abs_builddir[@]|$(abs_builddir)|g'				  \
  -e 's|@abs_srcdir[@]|$(abs_srcdir)|g'					  \
  -e 's|@abs_top_builddir[@]|$(abs_top_builddir)|g'			  \
  -e 's|@abs_top_srcdir[@]|$(abs_top_srcdir)|g'				  \
  -e 's|@bindir[@]|$(bindir)|g'						  \
  -e 's|@builddir[@]|$(builddir)|g'					  \
  -e 's|@configure_input[@]|Generated from $@.in; do not edit by hand.|g' \
  -e 's|@datadir[@]|$(pkgdatadir)|g'					  \
  -e 's|@prefix[@]|$(prefix)|g'						  \
  -e 's|@srcdir[@]|$(srcdir)|g'						  \
  -e 's|@top_builddir[@]|$(top_builddir)|g'				  \
  -e 's|@top_srcdir[@]|$(top_srcdir)|g'

# Scripts written in M4sh.
m4sh_dependencies =				\
  $(build_aux_dir)/base.as			\
  $(build_aux_dir)/rst.as			\
  $(build_aux_dir)/instrument.as		\
  $(build_aux_dir)/children.as			\
  $(build_aux_dir)/urbi.as

EXTRA_DIST += $(m4sh_dependencies)

%.in: $(srcdir)/%.as $(m4sh_dependencies)
	autom4te --language M4sh $(m4sh_dependencies) $< -o $@

## All the scripts depend on Makefile so that they are rebuilt when the
## prefix etc. changes.  It took quite a while to have the rule correct,
## don't break it!
## Use chmod -w to prevent people from editing the wrong file by accident.
$(m4sh_scripts): %: %.in Makefile
	rm -f $@ $@.tmp
	$(edit) `test -f ./$@.in || echo $(srcdir)/`$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

CLEANFILES += $(m4sh_scripts:=.in)
