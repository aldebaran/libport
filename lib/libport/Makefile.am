include $(top_srcdir)/build-aux/init.mk

## --------- ##
## libport.  ##
## --------- ##

lib_libport = .
include libport-sources.mk

# Can be useful for some.  Others will use libport_sources directly.
lib_LTLIBRARIES += libport.la
dist_libport_la_SOURCES = $(libport_sources)
libport_la_LDFLAGS = -no-undefined
AM_CPPFLAGS += $(LIBPORT_CPPFLAGS) -DBUILDING_LIBPORT $(BOOST_CPPFLAGS)


# Make sure noone uses config.h instead of libport/config.h.
maintainer-check:
	cd $(srcdir) && ! grep -E '[<"]config.h[">]' $(nobase_libport_HEADERS) $(libport_HEADERS)
# GNU Make 3.80 does not recognize that `!' is a shell builtin, and
# tries to call it via exec.  So help GNU Make understand it should
# call the shell.
	: && ! grep -E '[<"]config.h[">]' $(libport_sources)
