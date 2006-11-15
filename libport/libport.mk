# The point of this file is to ship *and* to install libport.

include $(top_srcdir)/build-aux/config-headers-prefixed.mk

## --------- ##
## libport.  ##
## --------- ##

# Where we find them in this package.
libport_srcdir = $(top_srcdir)/libport
libport_builddir = $(top_builddir)/libport
# Where we install them.
libportdir = $(includedir)/libport

libport_HEADERS = 				\
$(libport_srcdir)/fwd.hh       		        \
$(libport_srcdir)/hash.hh			\
$(libport_srcdir)/lockable.hh			\
$(libport_srcdir)/semaphore.hh			\
$(libport_srcdir)/singleton-ptr.hh		\
$(libport_srcdir)/string.hh			\
$(libport_srcdir)/thread.hh			\
$(libport_srcdir)/ufloat.h			\
$(libport_srcdir)/ufloat.hh			\
$(libport_srcdir)/uffloat.hh			\
$(libport_srcdir)/ull-fixed-point.hh		\
$(libport_srcdir)/ulong-fixed-point.hh		\
$(libport_srcdir)/utime.hh

nodist_libport_HEADERS = 			\
$(libport_builddir)/config.h

libport_sources = 				\
$(libport_srcdir)/ufloat.cc			\
$(libport_srcdir)/uffloat.cc			\
$(libport_srcdir)/ull-fixed-point.cc


## ------------ ##
## svn sugars.  ##
## ------------ ##

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
