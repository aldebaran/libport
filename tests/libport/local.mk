include $(top_srcdir)/build-aux/instrument.mk
# This is for Boost Unit Test tests.  "all" is much more verbose.
LOG_LEVEL = test_suite
TESTSFLAGS = --log_level=$(LOG_LEVEL)

# For some reason, this rule *must* be defined before loading
# check.mk.
%.log: %$(EXEEXT)
	@$(am__check_pre) $(RUN_INSTRUMENT) ./$< $(TESTSFLAGS) $(am__check_post)

# Program to check:
TESTS_BINARIES =                                \
  tests/libport/asio.cc                         \
  tests/libport/assert.cc                       \
  tests/libport/attributes.cc                   \
  tests/libport/base64.cc                       \
  tests/libport/backtrace.cc                    \
  tests/libport/cli.cc                          \
  tests/libport/cmath.cc                        \
  tests/libport/condition.cc                    \
  tests/libport/containers.cc                   \
  tests/libport/cstdlib.cc                      \
  tests/libport/damerau-levenshtein-distance.cc \
  tests/libport/debug.cc                        \
  tests/libport/debug-dummy.cc                  \
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
  tests/libport/hash.cc                         \
  tests/libport/hmac-sha1.cc			\
  tests/libport/indent.cc                       \
  tests/libport/input-arguments.cc		\
  tests/libport/intrusive-ptr.cc                \
  tests/libport/io-stream.cc                    \
  tests/libport/markup-ostream.cc               \
  tests/libport/option-parser.cc		\
  tests/libport/path.cc                         \
  tests/libport/pid-file.cc                     \
  tests/libport/preproc.cc                      \
  tests/libport/pthread.cc                      \
  tests/libport/read-stdin.cc                   \
  tests/libport/safe-container.cc               \
  tests/libport/semaphore.cc                    \
  tests/libport/separate.cc                     \
  tests/libport/singleton-ptr.cc                \
  tests/libport/statistics.cc                   \
  tests/libport/symbol.cc                       \
  tests/libport/synchronizer.cc			\
  tests/libport/thread-pool.cc			\
  tests/libport/time.cc                         \
  tests/libport/timer.cc                        \
  tests/libport/tokenizer.cc                    \
  tests/libport/traits.cc			\
  tests/libport/ufloat-double.cc                \
  tests/libport/unescape.cc                     \
  tests/libport/utime.cc			\
  tests/libport/utsname.cc			\
  tests/libport/xalloc.cc			\
  tests/libport/xltdl.cc

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

AM_CPPFLAGS +=					\
  -I $(srcdir)/tests
AM_LDFLAGS +=                                   \
  $(BOOST_SERIALIZATION_LDFLAGS)                \
  $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
LDADD +=                                        \
  $(LIBPORT_LIBS)                               \
  $(BOOST_SERIALIZATION_LIBS)                   \
  $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)


tests_libport_asio_LDADD = $(BOOST_SYSTEM_LIBS)  $(LDADD)
tests_libport_asio_LDFLAGS = $(BOOST_SYSTEM_LDFLAGS) $(AM_LDFLAGS)
tests_libport_asio_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)

tests_libport_xltdl_LDADD = $(LDADD) $(LTDL_LIBS)

EXTRA_DIST +=					\
  tests/libport/test.hh				\
  tests/libport/ufloat-common.cc		\
  tests/libport/ufloat-config.h			\
  tests/libport/666.txt

TESTS_ENVIRONMENT +=                            \
  SRCDIR=$(srcdir)

CLEANFILES += tests/libport/exists.pid
