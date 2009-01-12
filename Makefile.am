include build-aux/init.mk
include build-aux/build-aux.mk
include build-aux/fix-libtool-la.mk

ACLOCAL_AMFLAGS = -I build-aux
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status $@

SUBDIRS = lib/libport doc tests
# SUBDIRS += gui
EXTRA_DIST += bootstrap


## -------------------- ##
## Libport components.  ##
## -------------------- ##

AM_CPPFLAGS += $(LIBPORT_CPPFLAGS) -I$(srcdir)/lib
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

include_libport = include/libport
include include/libport/local.mk

include include/sched/local.mk
include lib/sched/local.mk


## --------------- ##
## Extra targets.  ##
## --------------- ##

.PHONY: doc html
doc html:
	$(MAKE) $(AM_MAKEFLAGS) -C doc $@

# The target run by the buildfarm.
CHECK_BUILDFARM_FLAGS = -j4 AM_COLOR_TESTS=no VERBOSE=1 INSTRUMENT=1
check-buildfarm:
	$(MAKE) $(AM_MAKEFLAGS) -C tests	\
	  $(CHECK_BUILDFARM_FLAGS) check-html

# Manually list the files you're using from build-aux here.
# Note: Automake already takes care of the .mk and .m4 files you use.
EXTRA_DIST += \
  build-aux/test.sh
