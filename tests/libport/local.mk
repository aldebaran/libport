# This is for Boost Unit Test tests.  "all" is much more verbose.
TESTSFLAGS = --log_level=test-suite

# There are issues with Boost
# (https://svn.boost.org/trac/boost/ticket/2889) that prevent us from
# using instrumentation with Boost.Test.
#
INSTRUMENT = $(build_aux_srcdir)/instrument
INSTRUMENTFLAGS =                                                      \
  --valgrind-option=--suppressions=$(build_aux_srcdir)/instrument.supp \
  --valgrind-option=--gen-suppressions=all                             \
  --libtool=$(abs_top_builddir)/libtool
RUN_INSTRUMENT = $(INSTRUMENT) $(INSTRUMENTFLAGS)

# For some reason, this rule *must* be defined before loading
# check.mk.
%.log: %$(EXEEXT)
	@$(am__check_pre) $(RUN_INSTRUMENT) ./$< $(TESTSFLAGS) $(am__check_post)

# Program to check:
TESTS_BINARIES =                                \
  tests/libport/asio.cc                         \
  tests/libport/assert.cc                       \
  tests/libport/base64.cc                       \
  tests/libport/backtrace.cc                    \
  tests/libport/cli.cc                          \
  tests/libport/condition.cc                    \
  tests/libport/cstdlib.cc                      \
  tests/libport/damerau-levenshtein-distance.cc \
  tests/libport/debug.cc                        \
  tests/libport/deref.cc                        \
  tests/libport/dirent.cc                       \
  tests/libport/erase-if.cc                     \
  tests/libport/escape.cc                       \
  tests/libport/fd-stream.cc                    \
  tests/libport/fifo.cc                         \
  tests/libport/file-library.cc                 \
  tests/libport/finally.cc                      \
  tests/libport/foreach.cc                      \
  tests/libport/has-if.cc                       \
  tests/libport/indent.cc                       \
  tests/libport/intrusive-ptr.cc                \
  tests/libport/io-stream.cc                    \
  tests/libport/markup-ostream.cc               \
  tests/libport/path.cc                         \
  tests/libport/pid-file.cc                     \
  tests/libport/pthread.cc                      \
  tests/libport/semaphore.cc                    \
  tests/libport/separator.cc                    \
  tests/libport/singleton-ptr.cc                \
  tests/libport/statistics.cc                   \
  tests/libport/symbol.cc                       \
  tests/libport/time.cc                         \
  tests/libport/timer.cc                        \
  tests/libport/tokenizer.cc                    \
  tests/libport/ufloat-double.cc                \
  tests/libport/unescape.cc                     \
  tests/libport/utime.cc

# ufloat-float.cc is commented out as we do not support ufloat /=
# double (it would require that we recompile the library with another
# definition of ufloat).

if WITH_BOOST_SERIALIZATION
TESTS_BINARIES += tests/libport/intrusive-ptr-serialize.cc
endif
# Tabulation does not seem to work: doesn't even compile.
#ufloat-float-tab.cc                            \
#ufloat-double-tab.cc                           \
#ufloat-floating-tab.cc
#ufloat-floating.cc

EXTRA_PROGRAMS = $(TESTS_BINARIES:.cc=)
CLEANFILES += $(EXTRA_PROGRAMS)

AM_LDFLAGS +=                                   \
  $(BOOST_SERIALIZATION_LDFLAGS)                \
  $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
LDADD +=                                        \
  $(LIBPORT_LIBS)                               \
  $(BOOST_SERIALIZATION_LIBS)                   \
  $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)


tests_libport_asio_SOURCES                        = tests/libport/asio.cc
tests_libport_asio_LDADD = $(BOOST_SYSTEM_LIBS)  $(LDADD)
tests_libport_asio_LDFLAGS = $(BOOST_SYSTEM_LDFLAGS) $(AM_LDFLAGS)
tests_libport_asio_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)

tests_libport_assert_SOURCES                            = tests/libport/assert.cc
tests_libport_base64_SOURCES                            = tests/libport/base64.cc
tests_libport_backtrace_SOURCES                         = tests/libport/backtrace.cc
tests_libport_cli_SOURCES                               = tests/libport/cli.cc
tests_libport_condition_SOURCES                         = tests/libport/condition.cc
tests_libport_cstdlib_SOURCES                           = tests/libport/cstdlib.cc
tests_libport_damerau_levenshtein_distance_SOURCES      = tests/libport/damerau-levenshtein-distance.cc
tests_libport_debug_SOURCES                             = tests/libport/debug.cc
tests_libport_deref_SOURCES                             = tests/libport/deref.cc
tests_libport_dirent_SOURCES                            = tests/libport/dirent.cc
tests_libport_erase_if_SOURCES                          = tests/libport/erase-if.cc
tests_libport_escape_SOURCES                            = tests/libport/escape.cc
tests_libport_fd_stream_SOURCES                         = tests/libport/fd-stream.cc
tests_libport_fifo_SOURCES                              = tests/libport/fifo.cc
tests_libport_file_library_SOURCES                      = tests/libport/file-library.cc
tests_libport_finally_SOURCES                           = tests/libport/finally.cc
tests_libport_foreach_SOURCES                           = tests/libport/foreach.cc
tests_libport_has_if_SOURCES                            = tests/libport/has-if.cc
tests_libport_indent_SOURCES                            = tests/libport/indent.cc
tests_libport_intrusive_ptr_SOURCES                     = tests/libport/intrusive-ptr.cc
tests_libport_intrusive_ptr_serialize_SOURCES           = tests/libport/intrusive-ptr-serialize.cc
tests_libport_io_stream_SOURCES                         = tests/libport/io-stream.cc
tests_libport_markup_ostream_SOURCES                    = tests/libport/markup-ostream.cc
tests_libport_path_SOURCES                              = tests/libport/path.cc
tests_libport_pid_file_SOURCES                          = tests/libport/pid-file.cc
tests_libport_pthread_SOURCES                           = tests/libport/pthread.cc
tests_libport_semaphore_SOURCES                         = tests/libport/semaphore.cc
tests_libport_separator_SOURCES                         = tests/libport/separator.cc
tests_libport_singleton_ptr_SOURCES                     = tests/libport/singleton-ptr.cc
tests_libport_statistics_SOURCES                        = tests/libport/statistics.cc
tests_libport_symbol_SOURCES                            = tests/libport/symbol.cc
tests_libport_time_SOURCES                              = tests/libport/time.cc
tests_libport_timer_SOURCES                             = tests/libport/timer.cc
tests_libport_tokenizer_SOURCES                         = tests/libport/tokenizer.cc
tests_libport_ufloat_double_SOURCES                     = tests/libport/ufloat-double.cc
tests_libport_unescape_SOURCES                          = tests/libport/unescape.cc
tests_libport_utime_SOURCES                             = tests/libport/utime.cc

EXTRA_DIST +=                                   \
  tests/libport/ufloat-common.cc                \
  tests/libport/ufloat-config.h                 \
  tests/libport/666.txt

TESTS_ENVIRONMENT +=                            \
  SRCDIR=$(srcdir)

CLEANFILES += tests/libport/exists.pid
