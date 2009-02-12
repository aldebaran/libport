##
## revision.mk: This file is part of build-aux.
## Copyright (C) 2006-2009, Gostai S.A.S.
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

## The following comments are obsolete, but contain facts that were
## hard to understand.
##
## Create the file git-config.hh with accurate revision information.
##
## We depend on .version to avoid frequent regeneration of this file.
##
## Alternatively you could make it "PHONY" in which case Make knows it
## has to recreate each time "make" is run.  Of course in that case you
## really want to use "move-if-change" to avoid updating the timestamp
## of this file, so that we don't have to recompile all its
## dependencies.
##
## But that does not work because in that case, as it is PHONY, when
## "recreated" the file is recent in the tables of Make (although its
## actual timestamp was not changed on the disk), so it wants to
## recompile all the files that depend on it.  In other words,
## "move-if-change" is useless :(
##
## Why don't we have the same problem with ".version"? (Indeed, it is
## declared PHONY, so recreated all the time, but then since
## git-version.hh depends on it, it will be recreated etc.).  Because
## it is not in the same Makefile (Makefile vs. src/Makefile).

REVISION = $(build_aux_dir)/git-version-gen
REVISION_PREFIX ?= PACKAGE_
BUILT_SOURCES += $(REVISION_FILE)
REVISION_FILE_STAMP = $(REVISION_FILE).stamp

EXTRA_DIST += $(REVISION) $(move_if_change)
CLEANFILES += $(REVISION_FILE) $(REVISION_FILE_STAMP)

$(REVISION_FILE_STAMP): $(REVISION) $(build_aux_dir)/revision.mk
	@rm -f $@.tmp
	@$(mkdir_p) $(dir $@)
	@touch $@.tmp
	$(REVISION)					\
		--cache=$(top_srcdir)/.version		\
		--prefix=$(REVISION_PREFIX)		\
		--srcdir=$(top_srcdir)			\
		--header --output=$(REVISION_FILE)
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
