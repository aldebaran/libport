# The point of this file is to ship (not to install) libport.  Only
# liburbi installs libport.  Other users simply want the files to be
# distributed, and to be given some sugar (e.g., libport_srcdir etc.).

include $(top_srcdir)/build-aux/config-headers-prefixed.mk

## --------- ##
## libport.  ##
## --------- ##

# Where we find them in this package.
libport_srcdir = $(top_srcdir)/libport
libport_builddir = $(top_builddir)/libport
# Where we install them.
libportdir = $(includedir)/libport

libport_headers = 				\
$(libport_srcdir)/fwd.hh       		        \
$(libport_srcdir)/hash.hh			\
$(libport_srcdir)/lockable.hh			\
$(libport_srcdir)/semaphore.hh			\
$(libport_srcdir)/singleton-ptr.hh		\
$(libport_srcdir)/thread.hh			\
$(libport_srcdir)/ufloat.h			\
$(libport_srcdir)/ufloat.hh			\
$(libport_srcdir)/uffloat.hh			\
$(libport_srcdir)/ull-fixed-point.hh		\
$(libport_srcdir)/ulong-fixed-point.hh		\
$(libport_srcdir)/utime.hh

nodist_libport_headers = 			\
$(libport_builddir)/config.h

libport_sources = 				\
$(libport_srcdir)/ufloat.cc			\
$(libport_srcdir)/uffloat.cc			\
$(libport_srcdir)/ull-fixed-point.cc

EXTRA_HEADERS += 				\
$(libport_headers)				\
$(libport_sources)


## ------------ ##
## svn sugars.  ##
## ------------ ##

.PHONY: libport-help svn-up svn-ci
help: libport-help
libport-help:
	@echo "svn-up:    upgrade in parent dir"
	@echo "svn-ci:    checkin and upgrade parent dir"


# Update a pinned svn:externals.
svn-up:
	$(svn_externals) --update=libport $(top_srcdir)

# Check in a pinned external from a host project, and depend upon it.
svn-ci:
	cd $(top_srcdir)/libport && $(SVN) ci
	$(MAKE) $(AM_MAKEFLAGS) svn-up
