include $(top_srcdir)/build-aux/init.mk

include $(top_srcdir)/build-aux/config-headers-prefixed.mk

## --------- ##
## libport.  ##
## --------- ##

include $(top_srcdir)/libport/libport-sources.mk

# C headers.
libport_HEADERS =				\
	cstdio					\
	cstring					\
	unistd.h

# C++ headers.
libport_HEADERS +=				\
	assert.hh				\
	cli.hh cli.hxx				\
	compiler.hh				\
	containers.hh containers.hxx		\
	contract.hh				\
	deref.hh				\
	deref.hxx				\
	detect_win32.h				\
	hash.hh					\
	indent.hh				\
	escape.hh escape.hxx			\
	exception.hh exception.hxx		\
	file-system.hh				\
	fwd.hh					\
	hash.hh					\
	lockable.hh				\
	network.h				\
	package-info.hh				\
	pair.hh pair.hxx			\
	program-name.hh				\
	ref-pt.hh				\
	select-const.hh				\
	select-ref.hh				\
	semaphore.hh				\
	semaphore.hxx				\
	separator.hh				\
	separator.hxx				\
	shared-ptr.hh				\
	shared-ptr.hxx				\
	singleton-ptr.hh			\
	symbol.hh				\
	symbol.hxx				\
	sysexits.hh				\
	thread.hh				\
	tokenizer.hh				\
	uffloat.hh				\
	ufloat.h				\
	ufloat.hh				\
	ull-fixed-point.hh			\
	ulong-fixed-point.hh			\
	utime.hh				\
	windows.hh

# sys/ headers
nobase_libport_HEADERS =			\
        sys/stat.h


# Generated headers.
nodist_libport_HEADERS = 			\
	$(builddir)/config.h


EXTRA_DIST += $(libport_sources)

# Can be useful for some.  Other will use libport_sources directly.
noinst_LTLIBRARIES = libport.la
libport_la_SOURCES = $(libport_sources)
AM_CPPFLAGS += -I$(top_builddir) -I$(top_srcdir) $(BOOST_CPPFLAGS)


# Make sure noone uses config.h instead of libport/config.h.
check-local:
	cd $(srcdir) && ! grep -E '[<"]config.h[">]' $(nobase_libport_HEADERS) $(libport_HEADERS)
# GNU Make 3.80 does not recognize that `!' is a shell builtin, and
# tries to call it via exec.  So help GNU Make understand it should
# call the shell.
	: && ! grep -E '[<"]config.h[">]' $(libport_sources)
