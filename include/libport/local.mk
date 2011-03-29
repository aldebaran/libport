## Copyright (C) 2008-2011, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

# C headers.
libport_includedir = $(includedir)/libport
libport_include_HEADERS =                       \
  include/libport/cassert                       \
  include/libport/cerrno                        \
  include/libport/cmath                         \
  include/libport/cmath.hxx                     \
  include/libport/csignal                       \
  include/libport/cstdint                       \
  include/libport/cstdio                        \
  include/libport/cstdlib                       \
  include/libport/cstdlib.hxx                   \
  include/libport/cstring                       \
  include/libport/cstring.hxx                   \
  include/libport/ctime                         \
  include/libport/fcntl.h                       \
  include/libport/pthread.h                     \
  include/libport/pthread.hxx                   \
  include/libport/semaphore.h                   \
  include/libport/unistd.h

# arpa/ headers
arpa_libport_includedir = $(libport_includedir)/arpa
arpa_libport_include_HEADERS =                  \
  include/libport/arpa/inet.h

# sys/ headers
sys_libport_includedir = $(libport_includedir)/sys
sys_libport_include_HEADERS =                   \
  include/libport/sys/param.h                   \
  include/libport/sys/prctl.h                   \
  include/libport/sys/select.h                  \
  include/libport/sys/socket.h                  \
  include/libport/sys/stat.h                    \
  include/libport/sys/stat.hxx                  \
  include/libport/sys/time.h                    \
  include/libport/sys/time.hxx                  \
  include/libport/sys/types.h                   \
  include/libport/sys/utsname.h                 \
  include/libport/sys/utsname.hxx               \
  include/libport/sys/wait.h


# C++ headers.
libport_include_HEADERS +=                              \
  include/libport/algorithm                             \
  include/libport/algorithm.hxx                         \
  include/libport/allocator-static.hh                   \
  include/libport/allocator-static.hxx                  \
  include/libport/asio.hh                               \
  include/libport/asio.hxx                              \
  include/libport/atomic.hh				\
  include/libport/attributes.hh                         \
  include/libport/backtrace.hh                          \
  include/libport/backtrace.hxx                         \
  include/libport/base64.hh                             \
  include/libport/bind.hh                               \
  include/libport/boost-error.hh                        \
  include/libport/boost-error.hxx                       \
  include/libport/buffer-stream.hh                      \
  include/libport/cast.hh                               \
  include/libport/cast.hxx                              \
  include/libport/cli.hh                                \
  include/libport/cli.hxx                               \
  include/libport/compilation.hh                        \
  include/libport/compiler.hh                           \
  include/libport/compiler-section.hh                   \
  include/libport/condition.hh                          \
  include/libport/condition.hxx                         \
  include/libport/containers.hh                         \
  include/libport/containers.hxx                        \
  include/libport/damerau-levenshtein-distance.hh       \
  include/libport/debug.hh                              \
  include/libport/debug.hxx                             \
  include/libport/deref.hh                              \
  include/libport/deref.hxx                             \
  include/libport/destructible.hh                       \
  include/libport/destructible.hxx                      \
  include/libport/detect-win32.h                        \
  include/libport/dirent.h                              \
  include/libport/dlfcn.h                               \
  include/libport/echo.hh                               \
  include/libport/echo.hxx                              \
  include/libport/errors.hh                             \
  include/libport/escape.hh                             \
  include/libport/escape.hxx                            \
  include/libport/exception.hh                          \
  include/libport/exception.hxx                         \
  include/libport/export.hh                             \
  include/libport/fd-stream.hh                          \
  include/libport/fifo.hh                               \
  include/libport/fifo.hxx                              \
  include/libport/file-library.hh                       \
  include/libport/file-library.hxx                      \
  include/libport/file-system.hh                        \
  include/libport/finally.hh                            \
  include/libport/finally.hxx                           \
  include/libport/fnmatch.h                             \
  include/libport/fnmatch.hxx                           \
  include/libport/foreach.hh                            \
  include/libport/fwd.hh                                \
  include/libport/hash.hh                               \
  include/libport/hierarchy.hh                          \
  include/libport/hmac-sha1.hh                          \
  include/libport/indent.hh                             \
  include/libport/input-arguments.hh                    \
  include/libport/input-arguments.hxx                   \
  include/libport/instance-tracker.hh                   \
  include/libport/instance-tracker.hxx                  \
  include/libport/instrument.hh                         \
  include/libport/intrusive-ptr.hh                      \
  include/libport/intrusive-ptr.hxx                     \
  include/libport/iostream                              \
  include/libport/io-stream.hh                          \
  include/libport/ip-semaphore.hh                       \
  include/libport/lexical-cast.hh                       \
  include/libport/local-data.hh                         \
  include/libport/local-data.hxx                        \
  include/libport/lockable.hh                           \
  include/libport/lockable.hxx                          \
  include/libport/map.hh                                \
  include/libport/map.hxx                               \
  include/libport/markup-ostream.hh                     \
  include/libport/meta.hh                               \
  include/libport/netdb.h                               \
  include/libport/network.h                             \
  include/libport/option-parser.hh                      \
  include/libport/option-parser.hxx                     \
  include/libport/package-info.hh                       \
  include/libport/pair.hh                               \
  include/libport/pair.hxx                              \
  include/libport/path.hh                               \
  include/libport/path.hxx                              \
  include/libport/pid-file.hh                           \
  include/libport/pod-cast.hh                           \
  include/libport/pod-cast.hxx                          \
  include/libport/preproc.hh                            \
  include/libport/program-name.hh                       \
  include/libport/range.hh                              \
  include/libport/range.hxx                             \
  include/libport/read-stdin.hh                         \
  include/libport/ref-counted.hh                        \
  include/libport/ref-counted.hxx                       \
  include/libport/ref-pt.hh                             \
  include/libport/reserved-vector.hh                    \
  include/libport/reserved-vector.hxx                   \
  include/libport/safe-container.hh                     \
  include/libport/safe-container.hxx                    \
  include/libport/sched.hh                              \
  include/libport/select-const.hh                       \
  include/libport/select-ref.hh                         \
  include/libport/semaphore.hh                          \
  include/libport/semaphore.hxx                         \
  include/libport/separate.hh                           \
  include/libport/separate.hxx                          \
  include/libport/separator.hh                          \
  include/libport/singleton-ptr.hh                      \
  include/libport/singleton-ptr.hxx                     \
  include/libport/specific-ptr.hh                       \
  include/libport/specific-ptr.hxx                      \
  include/libport/statistics.hh                         \
  include/libport/statistics.hxx                        \
  include/libport/symbol.hh                             \
  include/libport/symbol.hxx                            \
  include/libport/synchronizer.hh                       \
  include/libport/synchronizer.hxx                      \
  include/libport/sysexits.hh                           \
  include/libport/system-warning-pop.hh                 \
  include/libport/system-warning-push.hh                \
  include/libport/thread.hh                             \
  include/libport/thread.hxx                            \
  include/libport/thread-data.hh                        \
  include/libport/thread-pool.hh                        \
  include/libport/throw-exception.hh                    \
  include/libport/time.hh                               \
  include/libport/time.hxx                              \
  include/libport/timer.hh                              \
  include/libport/timer.hxx                             \
  include/libport/tokenizer.hh                          \
  include/libport/traits.hh                             \
  include/libport/type-info.hh                          \
  include/libport/type-info.hxx                         \
  include/libport/typelist.hh                           \
  include/libport/uffloat.hh                            \
  include/libport/ufloat.h                              \
  include/libport/ufloat.hh                             \
  include/libport/ufloat.hxx                            \
  include/libport/ull-fixed-point.hh                    \
  include/libport/ulong-fixed-point.hh                  \
  include/libport/unique-pointer.hh                     \
  include/libport/unique-pointer.hxx                    \
  include/libport/unit-test.hh                          \
  include/libport/utime.hh                              \
  include/libport/utime.hxx                             \
  include/libport/valgrind.hh                           \
  include/libport/version.hh                            \
  include/libport/vector.hh                             \
  include/libport/warning-pop.hh                        \
  include/libport/warning-push.hh                       \
  include/libport/weak-ptr.hh                           \
  include/libport/weak-ptr.hxx                          \
  include/libport/windows.hh                            \
  include/libport/xalloc.hh                             \
  include/libport/xalloc.hxx                            \
  include/libport/xltdl.hh                              \
  include/libport/xltdl.hxx

## ----------- ##
## generated.  ##
## ----------- ##

nodist_libport_include_HEADERS =		\
  include/libport/config.h			\
  include/libport/format.hh			\
  include/libport/revision.hh
CLEANFILES += $(nodist_libport_include_HEADERS)
BUILT_SOURCES += $(nodist_libport_include_HEADERS)

# config.h.
generate_libport_config_h = \
  $(srcdir)/include/libport/generate-libport-config-h
EXTRA_DIST += $(generate_libport_config_h)
# There is no point in maintaining a stamp file, Automake already does
# this for config.h.  Arguably, we could use _configs.sed, but it is
# not documented.
include/libport/config.h: $(CONFIG_HEADER) $(generate_libport_config_h)
	$(AM_V_GEN)rm -f $@ $@.tmp
	$(AM_V_at)test -d $$(dirname $@) || $(mkdir_p) $$(dirname $@)
	$(AM_V_at)$(generate_libport_config_h) $< $@.tmp
	$(AM_V_at)mv $@.tmp $@

# format.hh.
EXTRA_DIST += include/libport/format.hh.gen
%: %.gen
	$(AM_V_GEN)mkdir -p $(dir $@)
	$(AM_V_at)$< > $@.tmp
	$(AM_V_at)chmod a-w $@.tmp
	$(AM_V_at)$(top_srcdir)/build-aux/bin/move-if-change --color $@.tmp $@
	$(AM_V_at)touch $@

# revision.hh
include/libport/revision.hh: $(top_srcdir)/.version $(VERSIONIFY)
	$(AM_V_GEN)$(VERSIONIFY_RUN) --prefix=LIBPORT_PACKAGE_ --header=$@

