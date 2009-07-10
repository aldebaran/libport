lib_LTLIBRARIES += lib/libport/libport.la
lib_libport_libport_la_LDFLAGS =		\
  -avoid-version -no-undefined

lib_libport_libport_la_LIBADD =			\
  $(LIBADD)					\
  $(BOOST_SYSTEM_LIBS)				\
  $(CLOCK_LIBS)					\
  $(LTDL_LIBS)                                  \
  $(SOCKETS_LIBS)				\
  $(PTHREAD_LDFLAGS)				\
  $(PTHREAD_LIBS)				\
  $(SSL_LIBS)

lib_libport_libport_la_CPPFLAGS =		\
  $(AM_CPPFLAGS)				\
  -Ilib						\
  -DBUILDING_LIBPORT

# Sources to compile to use libport.
dist_lib_libport_libport_la_SOURCES = 		\
  lib/libport/asio.cc                           \
  lib/libport/assert.cc				\
  lib/libport/backtrace.cc			\
  lib/libport/base64.cc				\
  lib/libport/buffer-stream.cc			\
  lib/libport/cli.cc				\
  lib/libport/compiler.cc			\
  lib/libport/contract.cc			\
  lib/libport/csignal.cc			\
  lib/libport/cstdio.cc				\
  lib/libport/cstdlib.cc			\
  lib/libport/damerau-levenshtein-distance.cc   \
  lib/libport/debug.cc				\
  lib/libport/deref.cc				\
  lib/libport/dirent.cc				\
  lib/libport/escape.cc				\
  lib/libport/fd-stream.cc			\
  lib/libport/file-library.cc			\
  lib/libport/file-system.cc			\
  lib/libport/indent.cc				\
  lib/libport/io-stream.cc			\
  lib/libport/ip-semaphore.cc			\
  lib/libport/markup-ostream.cc			\
  lib/libport/option-parser.cc			\
  lib/libport/package-info.cc			\
  lib/libport/path.cc				\
  lib/libport/pid-file.cc			\
  lib/libport/program-name.cc			\
  lib/libport/read-stdin.cc			\
  lib/libport/sched.cc				\
  lib/libport/semaphore-rpl.cc			\
  lib/libport/semaphore.cc			\
  lib/libport/symbol.cc				\
  lib/libport/sysexits.cc			\
  lib/libport/time.cc				\
  lib/libport/timer.cc				\
  lib/libport/tokenizer.cc			\
  lib/libport/ufloat.cc				\
  lib/libport/unique-pointer.cc			\
  lib/libport/unistd.cc				\
  lib/libport/utime.cc				\
  lib/libport/version.cc			\
  lib/libport/xltdl.cc

# These are broken and someone will have to fix them...
# libport_sources += 				\
#   $(libport_srcdir)/uffloat.cc		\
#   $(libport_srcdir)/ull-fixed-point.cc

# Create the file git-config.hh with accurate revision information.
#
# We depend on .version to avoid frequent regeneration of this file.
REVISION_FILE = lib/libport/revision.hh
REVISION_PREFIX = LIBPORT_PACKAGE_
include $(top_srcdir)/build-aux/revision.mk

# Make sure nobody uses config.h instead of libport/config.h.
#maintainer-check:
#	cd $(srcdir) && ! grep -E '[<"]config.h[">]' $(nobase_libport_HEADERS) $(libport_HEADERS)
# GNU Make 3.80 does not recognize that `!' is a shell builtin, and
# tries to call it via exec.  So help GNU Make understand it should
# call the shell.
#	: && ! grep -E '[<"]config.h[">]' $(dist_lib_libport_libport_la_SOURCES)
