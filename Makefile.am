## Copyright (C) 2006, 2008-2010, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

include build-aux/init.mk
include build-aux/build-aux.mk
include build-aux/package-version.mk
include build-aux/fix-libtool-la.mk
include build-aux/libport.mk
include build-aux/make/build-farm.mk
include build-aux/bootstrap.mk
include build-aux/m4sh.mk

VERSION_STUBS = lib/libport/.libs/libport$(SHLIBEXT)

TESTS =
include build-aux/check.mk

ACLOCAL_AMFLAGS = -I build-aux
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status $@

EXTRA_DIST += 					\
  bootstrap					\
  LICENSE

include bin/local.mk
include doc/local.mk
include tests/local.mk

## -------------------- ##
## Libport components.  ##
## -------------------- ##

# Change these last, since some of the include files add paths to
# AM_CPPFLAGS, and
AM_CPPFLAGS +=					\
  $(LIBPORT_CPPFLAGS)				\
  -I$(srcdir)/lib
AM_CXXFLAGS += $(WARNING_CXXFLAGS) $(PTHREAD_CFLAGS)

# libport.
include include/libport/local.mk
include lib/libport/local.mk

# libserialize.
if ENABLE_SERIALIZATION
include include/serialize/local.mk

include lib/serialize/local.mk
endif

# libsched.
include include/sched/local.mk
include lib/sched/local.mk

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

# The targets run by the buildfarm.  Yes, it run installcheck, but it
# makes no sense for Libport, so just bounce to regular check.
CHECK_BUILDFARM_FLAGS = -j4 AM_COLOR_TESTS=no VERBOSE=1 INSTRUMENTATION=1
check-buildfarm installcheck-buildfarm: clean-semaphores
	$(MAKE) $(AM_MAKEFLAGS)			\
	  $(CHECK_BUILDFARM_FLAGS) check-html

# Manually list the files you're using from build-aux here.
# Note: Automake already takes care of the .mk and .m4 files you use.
EXTRA_DIST += \
  build-aux/test.sh
