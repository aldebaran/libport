include $(top_srcdir)/build-aux/init.mk
include $(top_srcdir)/build-aux/svn-externals.mk

EXTRA_DIST += $(top_srcdir)/build-aux/install-sh-c
build_aux_dir = $(top_srcdir)/build-aux


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

# Short cuts
.PHONY: baux-help baux-up baux-ci baux-pin
baux-up baux-ci baux-pin:
	$(MAKE) $(AM_MAKEFLAGS) $$(echo "$@" | sed s/baux/build-aux/)

SVN_EXTERNALS += build-aux

## -------- ##
## boost.m4 ##
## -------- ##

FETCH_BM4 = wget -O $(build_aux_dir)/boost.m4
boost-m4-up:
	$(FETCH_BM4) 'http://repo.or.cz/w/boost.m4.git?a=blob_plain;f=build-aux/boost.m4;hb=HEAD'
