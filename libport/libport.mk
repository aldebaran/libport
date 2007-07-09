# The point of this file is to ship *and* to install libport.  It must
# be included by the top level Makefile.am.  It is not another
# Makefile.am to spare Autotools cycles, but it might be time now for
# a more traditional appraoch.

include $(top_srcdir)/build-aux/config-headers-prefixed.mk

## --------- ##
## libport.  ##
## --------- ##

include $(top_srcdir)/libport/libport-sources.mk

# C headers.
libport_HEADERS = 				\
	$(libport_srcdir)/cstdio		\
	$(libport_srcdir)/cstring		\
	$(libport_srcdir)/unistd.h

libportsys_HEADERS =				\
        $(libport_srcdir)/sys/stat.h

# C++ headers.
libport_HEADERS +=				\
	$(libport_srcdir)/compiler.hh		\
	$(libport_srcdir)/assert.hh		\
	$(libport_srcdir)/containers.hh		\
	$(libport_srcdir)/containers.hxx	\
	$(libport_srcdir)/detect_win32.h	\
	$(libport_srcdir)/hash.hh		\
	$(libport_srcdir)/indent.hh		\
	$(libport_srcdir)/escape.hh		\
	$(libport_srcdir)/escape.hxx		\
	$(libport_srcdir)/exception.hxx		\
	$(libport_srcdir)/exception.hh		\
	$(libport_srcdir)/fwd.hh		\
	$(libport_srcdir)/hash.hh		\
	$(libport_srcdir)/lockable.hh		\
	$(libport_srcdir)/network.h		\
	$(libport_srcdir)/ref-pt.hh		\
	$(libport_srcdir)/select-const.hh	\
	$(libport_srcdir)/semaphore.hh		\
	$(libport_srcdir)/semaphore.hxx		\
	$(libport_srcdir)/shared-ptr.hh		\
	$(libport_srcdir)/shared-ptr.hxx	\
	$(libport_srcdir)/singleton-ptr.hh	\
	$(libport_srcdir)/symbol.hh		\
	$(libport_srcdir)/symbol.hxx		\
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


EXTRA_DIST += $(libport_sources)


## ------------ ##
## svn sugars.  ##
## ------------ ##

# svn-externals.mk must be included.
SVN_EXTERNALS += libport
