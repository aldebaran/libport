include $(top_srcdir)/build-aux/init.mk
include $(top_srcdir)/build-aux/config-headers-prefixed.mk

## --------- ##
## libport.  ##
## --------- ##

lib_libport = lib/libport
include $(top_srcdir)/lib/libport/libport-sources.mk

EXTRA_DIST += $(libport_sources)

# Can be useful for some.  Others will use libport_sources directly.
noinst_LTLIBRARIES = libport.la
libport_la_SOURCES = $(libport_sources)
AM_CPPFLAGS += $(LIBPORT_CPPFLAGS) $(BOOST_CPPFLAGS)


# Make sure noone uses config.h instead of libport/config.h.
maintainer-check:
	cd $(srcdir) && ! grep -E '[<"]config.h[">]' $(nobase_libport_HEADERS) $(libport_HEADERS)
# GNU Make 3.80 does not recognize that `!' is a shell builtin, and
# tries to call it via exec.  So help GNU Make understand it should
# call the shell.
	: && ! grep -E '[<"]config.h[">]' $(libport_sources)
