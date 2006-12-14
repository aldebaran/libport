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
	$(libport_srcdir)/containers.hh		\
	$(libport_srcdir)/containers.hxx	\
	$(libport_srcdir)/detect_win32.h	\
	$(libport_srcdir)/fwd.hh		\
	$(libport_srcdir)/hash.hh		\
	$(libport_srcdir)/lockable.hh		\
	$(libport_srcdir)/network.h		\
	$(libport_srcdir)/semaphore.hh		\
	$(libport_srcdir)/semaphore.hxx		\
	$(libport_srcdir)/singleton-ptr.hh	\
	$(libport_srcdir)/thread.hh		\
	$(libport_srcdir)/uffloat.hh		\
	$(libport_srcdir)/ufloat.h		\
	$(libport_srcdir)/ufloat.hh		\
	$(libport_srcdir)/ull-fixed-point.hh	\
	$(libport_srcdir)/ulong-fixed-point.hh	\
	$(libport_srcdir)/utime.hh		\
	$(libport_srcdir)/windows.hh

# Generated headers.
nodist_libport_HEADERS = 			\
	$(libport_builddir)/config.h

# Sources to compile to use libport.
libport_sources = 				\
	$(libport_srcdir)/ufloat.cc		\
	$(libport_srcdir)/uffloat.cc		\
	$(libport_srcdir)/ull-fixed-point.cc


## ------------ ##
## svn sugars.  ##
## ------------ ##

# svn-externals.mk must be included.
SVN_EXTERNALS += libport
