include build-aux/init.mk
include build-aux/build-aux.mk
include build-aux/package-version.mk
include build-aux/fix-libtool-la.mk
include build-aux/check.mk
TESTS =

ACLOCAL_AMFLAGS = -I build-aux
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status $@

# The libraries are in ".".
SUBDIRS = . tests
# SUBDIRS += gui
EXTRA_DIST += bootstrap

include doc/local.mk

## -------------------- ##
## Libport components.  ##
## -------------------- ##

# Change these last, since some of the include files add paths to
# AM_CPPFLAGS, and
AM_CPPFLAGS +=					\
  $(LIBPORT_CPPFLAGS)				\
  -I$(srcdir)/lib
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

# libport.
include include/libport/local.mk
include lib/libport/local.mk

# libserialize.
include include/serialize/local.mk
include lib/serialize/local.mk

# libsched.
include include/sched/local.mk
include lib/sched/local.mk

# libtinyxml.
include include/tinyxml/local.mk
include lib/tinyxml/local.mk

# libltdl.
include libltdl/Makefile.inc

# cl.exe wrappers.
include msvc/local.mk

# We don't want to have -I that go into /usr/local too soon:
# /opt/local headers would take precedence over ours (e.g., in the
# case of an installed libltdl).
AM_CPPFLAGS += $(BOOST_CPPFLAGS)


## ------- ##
## Tests.  ##
## ------- ##

TEST_LOGS = $(addsuffix .log,$(basename $(TESTS)))
check-clean-local:
	rm -rf $(TEST_LOGS:.log=.dir)


## --------------- ##
## Extra targets.  ##
## --------------- ##

.PHONY: doc html
doc html:
	$(MAKE) $(AM_MAKEFLAGS) -C doc $@

# The target run by the buildfarm.
CHECK_BUILDFARM_FLAGS = -j4 AM_COLOR_TESTS=no VERBOSE=1 INSTRUMENT=1
check-buildfarm:
	$(MAKE) $(AM_MAKEFLAGS)			\
	  $(CHECK_BUILDFARM_FLAGS) check-html
	$(MAKE) $(AM_MAKEFLAGS) -C tests	\
	  $(CHECK_BUILDFARM_FLAGS) check-html

# Manually list the files you're using from build-aux here.
# Note: Automake already takes care of the .mk and .m4 files you use.
EXTRA_DIST += \
  build-aux/test.sh
