# We install it in includedir, that easier currently.  Some
# day everybody should move into includedir/urbi.

# Where we find them in this package.
libport_srcdir = $(top_srcdir)/libport
# Where we install them.
libportdir = $(includedir)/libport

libport_HEADERS = 				\
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

libport_sources = 				\
$(libport_srcdir)/ufloat.cc			\
$(libport_srcdir)/uffloat.cc			\
$(libport_srcdir)/ull-fixed-point.cc


include $(top_srcdir)/build-aux/config-headers-prefixed.mk
