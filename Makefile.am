include build-aux/build-aux.mk

ACLOCAL_AMFLAGS = -I build-aux
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

SUBDIRS = lib/libport doc tests
# SUBDIRS += gui
EXTRA_DIST += bootstrap

include_libport = include/libport
include include/libport/local.mk

.PHONY: doc html
doc html:
	cd doc && $(MAKE) $(AM_MAKEFLAGS) $@

check-buildfarm: check

# Manually list the files you're using from build-aux here.
# Note: Automake already takes care of the .mk and .m4 files you use.
EXTRA_DIST += \
  build-aux/test.sh
