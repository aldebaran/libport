include $(top_srcdir)/libport/build-aux/init.mk

include $(top_srcdir)/libport/build-aux/config-headers-prefixed.mk

## --------- ##
## libport.  ##
## --------- ##

include $(top_srcdir)/libport/lib/libport-sources.mk

EXTRA_DIST += $(libport_sources)

# Can be useful for some.  Other will use libport_sources directly.
noinst_LTLIBRARIES = libport.la
libport_la_SOURCES = $(libport_sources)
AM_CPPFLAGS += -I$(top_builddir)/libport/include -I$(top_srcdir)/libport/include $(BOOST_CPPFLAGS)


# Make sure noone uses config.h instead of libport/config.h.
maintainer-check:
	cd $(srcdir) && ! grep -E '[<"]config.h[">]' $(nobase_libport_HEADERS) $(libport_HEADERS)
# GNU Make 3.80 does not recognize that `!' is a shell builtin, and
# tries to call it via exec.  So help GNU Make understand it should
# call the shell.
	: && ! grep -E '[<"]config.h[">]' $(libport_sources)
