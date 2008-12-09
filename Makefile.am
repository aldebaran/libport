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
	$(MAKE) $(AM_MAKEFLAGS) -C doc $@

# The target run by the buildfarm.
CHECK_BUILDFARM_FLAGS = -j4 AM_COLOR_TESTS=no VERBOSE=1 INSTRUMENT=1
check-buildfarm:
        $(MAKE) $(AM_MAKEFLAGS) -C src/tests \
          $(CHECK_BUILDFARM_FLAGS) check-html

# Manually list the files you're using from build-aux here.
# Note: Automake already takes care of the .mk and .m4 files you use.
EXTRA_DIST += \
  build-aux/test.sh
