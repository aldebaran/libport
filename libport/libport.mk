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

# C headers.
libport_HEADERS = 				\
$(libport_srcdir)/cstring

# C++ headers.
libport_HEADERS += 				\
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

# Generated headers.
nodist_libport_HEADERS = 			\
$(libport_builddir)/config.h

# Sources to compile to use libport.
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
	@ext=$(@:-help=);
	echo "$$ext-up:    update the svn:externals revision for $$ext";
	echo "$$ext-ci:    check in $$ext and up it";
	echo "$$ext-pin:   subscribe to the latest $$ext"

# Update the pinned external.
libport-up:
	$(svn_externals) --update=$(@:-up=) $(srcdir)

# Checkin the pinned external and update it.
libport-ci:
	cd $(srcdir)/$(@:-up=) && $(SVN) ci
	$(MAKE) $(AM_MAKEFLAGS) $(@:-up=-ci)

# Pin the svn:external by subscribing to it.
libport-pin:
	$(svn_externals) --subscribe=$(@:-pin=) $(srcdir)
