## Copyright (C) 2009-2011, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

lib_LTLIBRARIES += lib/libport/libport@LIBSFX@.la
lib_libport_libport@LIBSFX@_la_LDFLAGS =	\
  -avoid-version -no-undefined			\
  $(BOOST_SIGNALS_LDFLAGS)			\
  $(BOOST_THREAD_LDFLAGS)

lib_libport_libport@LIBSFX@_la_LIBADD =		\
  $(BOOST_FILESYSTEM_LIBS)			\
  $(BOOST_SIGNALS_LIBS)				\
  $(BOOST_SYSTEM_LIBS)				\
  $(BOOST_THREAD_LIBS)				\
  $(CLOCK_LIBS)					\
  $(LIBADD)					\
  $(LTDL_LIBS)					\
  $(PTHREAD_LDFLAGS)				\
  $(PTHREAD_LIBS)				\
  $(SOCKETS_LIBS)				\
  $(SSL_LIBS)

lib_libport_libport@LIBSFX@_la_CPPFLAGS =	\
  $(AM_CPPFLAGS)				\
  -Ilib						\
  -DBUILDING_LIBPORT

# Sources to compile to use libport.
dist_lib_libport_libport@LIBSFX@_la_SOURCES =	\
  lib/libport/asio.cc				\
  lib/libport/asio-impl.hxx			\
  lib/libport/asio-ssl.cc			\
  lib/libport/backtrace.cc			\
  lib/libport/base64.cc				\
  lib/libport/buffer-stream.cc			\
  lib/libport/cli.cc				\
  lib/libport/csignal.cc			\
  lib/libport/cstdio.cc				\
  lib/libport/cstdlib.cc			\
  lib/libport/cstring.cc			\
  lib/libport/ctime.cc				\
  lib/libport/damerau-levenshtein-distance.cc	\
  lib/libport/debug.cc				\
  lib/libport/deref.cc				\
  lib/libport/dirent.cc				\
  lib/libport/echo.cc				\
  lib/libport/escape.cc				\
  lib/libport/fd-stream.cc			\
  lib/libport/file-library.cc			\
  lib/libport/file-system.cc			\
  lib/libport/fnmatch.cc    			\
  lib/libport/format.cc				\
  lib/libport/hmac-sha1.cc			\
  lib/libport/indent.cc				\
  lib/libport/input-arguments.cc		\
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
  lib/libport/synchronizer.cc			\
  lib/libport/sys/utsname.cc			\
  lib/libport/sysexits.cc			\
  lib/libport/timer.cc				\
  lib/libport/thread-pool.cc			\
  lib/libport/tokenizer.cc			\
  lib/libport/ufloat.cc				\
  lib/libport/unique-pointer.cc			\
  lib/libport/unistd.cc				\
  lib/libport/utime.cc				\
  lib/libport/version.cc			\
  lib/libport/xalloc.cc				\
  lib/libport/xltdl.cc

nodist_lib_libport_libport@LIBSFX@_la_SOURCES =	\
  lib/libport/revision-stub.hh
BUILT_SOURCES += $(nodist_lib_libport_libport@LIBSFX@_la_SOURCES)
lib/libport/revision-stub.hh: $(VERSIONIFY)
	$(AT_V_GEN) $(VERSIONIFY_RUN) --prefix=LIBPORT_PACKAGE_ --stub=$@

# These are broken and someone will have to fix them...
# libport_sources += 				\
#   $(libport_srcdir)/uffloat.cc		\
#   $(libport_srcdir)/ull-fixed-point.cc

# Resolve stubs in libport.
all-local: lib/libport/libport.unstub.stamp
lib/libport/libport.unstub.stamp: lib/libport/libport$(LIBSFX).la $(VERSIONIFY_CACHE) $(VERSIONIFY)
	$(AT_V_PATCH) $(VERSIONIFY_RUN) $(if $(V:0=),-v,-q) --resolve=$<

# Make sure nobody uses config.h instead of libport/config.h.
#maintainer-check:
#	cd $(srcdir) && ! grep -E '[<"]config.h[">]' $(nobase_libport_HEADERS) $(libport_HEADERS)
# GNU Make 3.80 does not recognize that `!' is a shell builtin, and
# tries to call it via exec.  So help GNU Make understand it should
# call the shell.
#	: && ! grep -E '[<"]config.h[">]' $(dist_lib_libport_libport@LIBSFX@_la_SOURCES)
