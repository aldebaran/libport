include $(top_srcdir)/build-aux/init.mk
EXTRA_DIST += $(top_srcdir)/build-aux/install-sh-c
build_aux_dir = $(top_srcdir)/build-aux
svn_externals = $(build_aux_dir)/svn-externals
SVN = svn-wrapper.sh

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

.PHONY: baux-help baux-up baux-ci baux-pin
.PHONY: build-aux-help build-aux-up build-aux-ci build-aux-pin

help: baux-help
baux-help build-aux-help:
	@echo "baux-up:    update the dependencies on build-aux"
	@echo "baux-ci:    checkin build-aux, and upgrade dependency"
	@echo "baux-pin:   subscribe to build-aux"

# Update a pinned svn:externals.
baux-up build-aux-up:
	$(svn_externals) --update=build-aux $(srcdir)

# Check in a pinned external from a host project, and depend upon it.
baux-ci build-aux-ci:
	cd $(build_aux_dir) && $(SVN) ci
	$(MAKE) $(AM_MAKEFLAGS) baux-up

# Pin the svn:external by subscribing to it.
baux-pin build-aux-pin:
	$(svn_externals) --subscribe=baux $(srcdir)


## --------- ##
## libport.  ##
## --------- ##

.PHONY: libport-help libport-up libport-ci libport-pin
help: libport-help
libport-help:
	@echo "libport-up:    same as baux-up, but for libport"
	@echo "libport-ci:    likewise"
	@echo "libport-pin:   likewise"

# Update a pinned svn:externals.
libport-up:
	$(svn_externals) --update=libport $(srcdir)

# Check in a pinned external from a host project, and depend upon it.
libport-ci:
	cd $(top_srcdir)/libport && $(SVN) ci
	$(MAKE) $(AM_MAKEFLAGS) libport-up

# Pin the svn:external by subscribing to it.
libport-pin:
	$(svn_externals) --subscribe=libport $(srcdir)


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
