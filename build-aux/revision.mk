##
## revision.mk: This file is part of build-aux.
## Copyright (C) 2006-2008, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

## Build $(REVISION_FILE) using svn/git information.
##
## We use a stamp to avoid updating the revision file too often to
## avoid useless re-compilations.

REVISION = $(build_aux_dir)/revision

BUILT_SOURCES += $(REVISION_FILE)
REVISION_FILE_STAMP = $(REVISION_FILE).stamp

EXTRA_DIST += $(REVISION) $(move_if_change)
CLEANFILES += $(REVISION_FILE) $(REVISION_FILE_STAMP)

$(REVISION_FILE_STAMP): $(REVISION) $(build_aux_dir)/revision.mk
	@rm -f $@.tmp
	@touch $@.tmp
	top_srcdir=$(top_srcdir) $(REVISION) - $(VERSION) >$(REVISION_FILE).tmp
	$(move_if_change) $(REVISION_FILE).tmp $(REVISION_FILE)
	@mv -f $@.tmp $@

$(REVISION_FILE): $(REVISION_FILE_STAMP)
	@if test -f $(REVISION_FILE); then :; else \
	  rm -f $(REVISION_FILE_STAMP); \
	  $(MAKE) $(AM_MAKEFLAGS) $(REVISION_FILE_STAMP); \
	fi

.PHONY: update-revision
update-revision:
	rm -f $(REVISION_FILE)
	$(MAKE) $(REVISION_FILE)
