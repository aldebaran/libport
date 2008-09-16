# Define include_libport to the path from the Makefile.am that
# includes this file, to this file.  E.g., if TOP/include/Makefile.am
# does
#
#   include $(top_srcdir)/include/libport/libport.mk
#
# then define include_libport = libport

# C headers.
libport_includedir = $(libport_include_basedir)/libport
libport_include_HEADERS =			\
	$(include_libport)/csignal		\
	$(include_libport)/cstdio		\
	$(include_libport)/cstring		\
	$(include_libport)/unistd.h

# C++ headers.
libport_include_HEADERS +=			\
	$(include_libport)/assert.hh		\
	$(include_libport)/cli.hh		\
	$(include_libport)/cli.hxx		\
	$(include_libport)/compilation.hh	\
	$(include_libport)/compiler.hh		\
	$(include_libport)/containers.hh	\
	$(include_libport)/containers.hxx	\
	$(include_libport)/contract.hh		\
	$(include_libport)/deref.hh		\
	$(include_libport)/deref.hxx		\
	$(include_libport)/detect-win32.h	\
	$(include_libport)/errors.hh		\
	$(include_libport)/escape.hh		\
	$(include_libport)/escape.hxx		\
	$(include_libport)/exception.hh		\
	$(include_libport)/exception.hxx	\
	$(include_libport)/export.hh		\
	$(include_libport)/fifo.hh		\
	$(include_libport)/fifo.hxx		\
	$(include_libport)/file-library.hh	\
	$(include_libport)/file-library.hxx	\
	$(include_libport)/file-system.hh	\
	$(include_libport)/finally.hh		\
	$(include_libport)/finally.hxx		\
	$(include_libport)/foreach.hh		\
	$(include_libport)/fwd.hh		\
	$(include_libport)/hash.hh		\
	$(include_libport)/indent.hh		\
	$(include_libport)/lexical-cast.hh	\
	$(include_libport)/lockable.hh		\
	$(include_libport)/network.h		\
	$(include_libport)/map.hh		\
	$(include_libport)/map.hxx		\
	$(include_libport)/meta.hh		\
	$(include_libport)/package-info.hh	\
	$(include_libport)/pair.hh		\
	$(include_libport)/pair.hxx		\
	$(include_libport)/path.hh		\
	$(include_libport)/path.hxx		\
	$(include_libport)/pod-cast.hh		\
	$(include_libport)/pod-cast.hxx		\
	$(include_libport)/program-name.hh	\
	$(include_libport)/range.hh		\
	$(include_libport)/range.hxx		\
	$(include_libport)/read-stdin.hh	\
	$(include_libport)/ref-counted.hh	\
	$(include_libport)/ref-counted.hxx	\
	$(include_libport)/ref-pt.hh		\
	$(include_libport)/sched.hh		\
	$(include_libport)/select-const.hh	\
	$(include_libport)/select-ref.hh	\
	$(include_libport)/semaphore.hh		\
	$(include_libport)/semaphore.hxx	\
	$(include_libport)/separator.hh		\
	$(include_libport)/separator.hxx	\
	$(include_libport)/shared-ptr.hh	\
	$(include_libport)/shared-ptr.hxx	\
	$(include_libport)/singleton-ptr.hh	\
	$(include_libport)/statistics.hh	\
	$(include_libport)/statistics.hxx	\
	$(include_libport)/symbol.hh		\
	$(include_libport)/symbol.hxx		\
	$(include_libport)/sysexits.hh		\
	$(include_libport)/thread.hh		\
	$(include_libport)/throw-exception.hh	\
	$(include_libport)/timer.hh		\
	$(include_libport)/timer.hxx		\
	$(include_libport)/tokenizer.hh		\
	$(include_libport)/traits.hh		\
	$(include_libport)/typelist.hh		\
	$(include_libport)/uffloat.hh		\
	$(include_libport)/ufloat.h		\
	$(include_libport)/ufloat.hh		\
	$(include_libport)/ufloat.hxx		\
	$(include_libport)/ull-fixed-point.hh	\
	$(include_libport)/unique-pointer.hh	\
	$(include_libport)/unique-pointer.hxx	\
	$(include_libport)/unit-test.hh		\
	$(include_libport)/ulong-fixed-point.hh	\
	$(include_libport)/utime.hh		\
	$(include_libport)/visitor.hh		\
	$(include_libport)/weak-ptr.hh		\
	$(include_libport)/weak-ptr.hxx		\
	$(include_libport)/windows.hh

# sys/ headers
sys_libport_includedir = $(libport_includedir)/sys
sys_libport_include_HEADERS =			\
        $(include_libport)/sys/param.h		\
        $(include_libport)/sys/stat.h


## ---------- ##
## config.h.  ##
## ---------- ##
nodist_libport_include_HEADERS =		\
	$(include_libport)/config.h

generate_libport_config_h = $(srcdir)/$(include_libport)/generate-libport-config-h
EXTRA_DIST += $(generate_libport_config_h)
# There is no point in maintaining a stamp file, Automake already does
# this for config.h.  Arguably, we could use _configs.sed, but it is
# not documented.
$(include_libport)/config.h: $(CONFIG_HEADER) $(generate_libport_config_h)
	rm -f $@ $@.tmp
	test -d $(include_libport) || $(mkdir_p) $(include_libport)
	$(generate_libport_config_h) $< $@.tmp
	mv $@.tmp $@

CLEANFILES += $(nodist_libport_include_HEADERS)
BUILT_SOURCES += $(nodist_libport_include_HEADERS)
