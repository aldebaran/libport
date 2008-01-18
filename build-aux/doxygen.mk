##
## doxygen.mk: This file is part of build-aux.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

## To use this module, DOCDIR must be defined.
## Additionnal variable: EXCLUDE_PATTERNS (see the Doxygen manual)

## ----------------------- ##
## Doxygen documentation.  ##
## ----------------------- ##

.PHONY: doc internal-doc user-doc

all-local: $(COND_DOC_OUTPUT)

doc: internal-doc

internal-doc:
	rm -rf $(DOCDIR).tmp $(builddir)/$(DOCDIR)
	$(MAKE) $(AM_MAKEFLAGS) DOC_INTERNAL=YES doc-builder

user-doc:
	rm -rf $(DOCDIR).tmp $(builddir)/$(DOCDIR)
	$(MAKE) $(AM_MAKEFLAGS) DOC_INTERNAL=NO doc-builder

EXTRA_DIST = $(builddir)/$(DOCDIR)

$(builddir)/$(DOCDIR):
	$(mkdir_p) $(builddir)/$(DOCDIR)

doc-builder: $(top_srcdir)/build-aux/Doxyfile.in
	test -w $(builddir)
	rm -rf $(DOCDIR).tmp $(builddir)/$(DOCDIR)
	$(MAKE) $(AM_MAKEFLAGS) Doxyfile
	sed -e 's/@DOC_INTERNAL@/$(DOC_INTERNAL)/g' \
            -e 's,@EXCLUDE_PATTERNS@,$(EXCLUDE_PATTERNS),g' Doxyfile >Doxytmp
	mv -f Doxytmp Doxyfile
	chmod -w Doxyfile
	$(DOXYGEN) Doxyfile
	mv $(DOCDIR).tmp $(builddir)/$(DOCDIR)

maintainer-clean-local:
	rm -rf $(DOCDIR).tmp $(builddir)/$(DOCDIR)

install-data-local: $(COND_DOC_INSTALL)

doc-install: internal-doc-install

# We install by hand, otherwise Automake produces "install .../srcdoc
# .../srcdoc", which install our dir into the previous one, instead of
# replacing it.
internal-doc-install:
	rm -rf $(DESTDIR)$(htmldir)/$(DOCDIR)
	$(mkdir_p) $(DESTDIR)$(htmldir)
	cp -r $(builddir)/$(DOCDIR) $(DESTDIR)$(htmldir)

uninstall-local: $(COND_DOC_UNINSTALL)

doc-uninstall: internal-doc-uninstall

internal-doc-uninstall:
	(test -d && chmod -R 700 $(DESTDIR)$(htmldir)/$(DOCDIR)) || true
	rm -rf $(DESTDIR)$(htmldir)/$(DOCDIR)
