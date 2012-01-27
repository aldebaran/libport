## Copyright (C) 2006, 2008-2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

include build-aux/make/init.mk
include build-aux/make/build-aux.mk
include build-aux/make/package-version.mk
include build-aux/make/fix-libtool-la.mk
include build-aux/make/libport.mk
include build-aux/make/build-farm.mk
include build-aux/make/bootstrap.mk
include build-aux/make/m4sh.mk

TESTS =
include build-aux/make/check.mk

ACLOCAL_AMFLAGS = -I build-aux/m4 -I build-aux
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
