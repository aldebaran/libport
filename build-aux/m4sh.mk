# Use autom4te to create our scripts, with additions stored in
# $(top_srcdir)build-aux.  Define m4sh_scripts.

## ------------- ##
## The scripts.  ##
## ------------- ##

edit = sed \
	-e 's|@SHELL[@]|$(SHELL)|g' \
	-e 's|@PERL[@]|$(PERL)|g' \
	-e 's|@bindir[@]|$(bindir)|g' \
	-e 's|@datadir[@]|$(pkgdatadir)|g' \
	-e 's|@prefix[@]|$(prefix)|g' \
	-e 's|@VERSION[@]|$(VERSION)|g' \
	-e 's|@PACKAGE_NAME[@]|$(PACKAGE_NAME)|g' \
	-e 's|@configure_input[@]|Generated from $@.in; do not edit by hand.|g'

# Scripts written in M4sh.
m4sh_dependencies =				\
$(build_aux_dir)/urbi.as			\
$(build_aux_dir)/rst.as				\
$(build_aux_dir)/instrument.as

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
