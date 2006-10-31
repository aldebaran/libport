EXTRA_DIST = $(top_srcdir)/build-aux/install-sh-c

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
	cd $(top_srcdir) && build-aux/svn-externals --update=build-aux

# Check in a pinned external from a host project, and depend upon it.
baux-ci build-aux-ci:
	cd $(top_srcdir)/build-aux && $(SVN) ci
	$(MAKE) $(AM_MAKEFLAGS) baux-up

# Pin the svn:external by subscribing to it.
baux-pin build-aux-pin:
	cd $(top_srcdir) && build-aux/svn-externals --subscribe=baux


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
	cd $(top_srcdir) && build-aux/svn-externals --update=libport

# Check in a pinned external from a host project, and depend upon it.
libport-ci:
	cd $(top_srcdir)/libport && $(SVN) ci
	$(MAKE) $(AM_MAKEFLAGS) libport-up

# Pin the svn:external by subscribing to it.
libport-pin:
	cd $(top_srcdir) && build-aux/svn-externals --subscribe=libport


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
