include $(top_srcdir)/build-aux/init.mk
EXTRA_DIST += $(top_srcdir)/build-aux/install-sh-c
build_aux_dir = $(top_srcdir)/build-aux
svn_externals = $(build_aux_dir)/svn-externals


## ------ ##
## Help.  ##
## ------ ##

.PHONY: help help-first

help: help-first
help-first:
	@echo "Some make targets:"


## --------- ##
## recheck.  ##
## --------- ##

# It is often helpful to rerun configure (well, config.status).
# This is a convenient shorthand.
.PHONY: recheck recheck-help
help: recheck-help
recheck-help:
	@echo "recheck:  rerun config.status"

recheck:
	cd $(top_builddir) && ./config.status --recheck
	cd $(top_builddir) && ./config.status
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS)


## ----------- ##
## Build-aux.  ##
## ----------- ##

.PHONY: build-aux-help build-aux-up build-aux-ci build-aux-pin
help: build-aux-help
build-aux-help:
	@ext=$(@:-help=);
	echo "$$ext-up:    update the svn:externals revision for $$ext";
	echo "$$ext-ci:    check in $$ext and up it";
	echo "$$ext-pin:   subscribe to the latest $$ext"

# Update the pinned external.
build-aux-up:
	$(svn_externals) --update=$(@:-up=) $(srcdir)

# Checkin the pinned external and update it.
build-aux-ci:
	cd $(srcdir)/$(@:-up=) && $(SVN) ci
	$(MAKE) $(AM_MAKEFLAGS) $(@:-up=-ci)

# Pin the svn:external by subscribing to it.
build-aux-pin:
	$(svn_externals) --subscribe=$(@:-pin=) $(srcdir)

# Short cuts
.PHONY: baux-help baux-up baux-ci baux-pin
baux-up baux-ci baux-pin:
	$(MAKE) $(AM_MAKEFLAGS) $$(echo "$@" | sed s/baux/build-aux/)


## ----- ##
## All.  ##
## ----- ##

.PHONY: all-up-help all-up

help: all-up-help
all-up-help:
	@echo "all-up:    update all the svn:externals dependencies"

# Update a pinned svn:externals.
all-up:
	cd $(top_srcdir) && build-aux/svn-externals --update-all
