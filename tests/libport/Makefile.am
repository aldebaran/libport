include $(top_srcdir)/build-aux/init.mk
include $(top_srcdir)/build-aux/m4sh.mk

AM_CPPFLAGS += $(LIBPORT_CPPFLAGS)
AM_CXXFLAGS += $(WARNING_CXXFLAGS)

# No need to re-run the test if the sources have not changed.
LAZY_TEST_SUITE = 1

# This is for Boost Unit Test tests.  "all" is much more verbose.
TESTSFLAGS = --log_level=test-suite
# For some reason, this rule *must* be defined before loading
# check.mk.
%.log: %$(EXEEXT)
	@$(am__check_pre) ./$< $(TESTSFLAGS) $(am__check_post)
include $(top_srcdir)/build-aux/check.mk

# Program to check:
TESTS =                                        \
  asio.cc					\
  base64.cc                                     \
  cli.cc                                        \
  cstdlib.cc                                    \
  deref.cc                                      \
  dirent.cc                                     \
  fd-stream.cc                                  \
  foreach.cc                                    \
  path.cc                                       \
  singleton-ptr.cc                              \
  symbol.cc                                     \
  erase-if.cc                                   \
  escape.cc                                     \
  fifo.cc                                       \
  finally.cc                                    \
  has-if.cc                                     \
  indent.cc                                     \
  intrusive-ptr.cc                              \
  semaphore.cc                                  \
  separator.cc                                  \
  pid-file.cc					\
  statistics.cc                                 \
  timer.cc                                      \
  unescape.cc                                   \
  ufloat-double.cc                              \
  visitor.cc

# This test has been commented out as we do not support ufloat /= double.
# ufloat-float.cc

if WITH_BOOST_SERIALIZATION
TESTS += intrusive-ptr-serialize.cc
endif
# Tabulation does not seem to work: doesn't even compile.
#ufloat-float-tab.cc                            \
#ufloat-double-tab.cc                           \
#ufloat-floating-tab.cc
#ufloat-floating.cc

TEST_LOGS = $(TESTS:.cc=.log)

EXTRA_PROGRAMS = $(TESTS:.cc=)
CLEANFILES += $(EXTRA_PROGRAMS)

AM_CPPFLAGS +=                                  \
  $(BOOST_CPPFLAGS)
AM_LDFLAGS +=                                   \
  $(BOOST_SERIALIZATION_LDFLAGS)                \
  $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
LDADD +=                                        \
  $(top_builddir)/lib/libport/libport.la        \
  $(BOOST_SERIALIZATION_LIBS)                   \
  $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

#ufloat_double_tab_SOURCES        = ufloat-double-tab.cc
#ufloat_float_SOURCES             = ufloat-float.cc
#ufloat_float_tab_SOURCES         = ufloat-float-tab.cc
#ufloat_floating_SOURCES          = ufloat-floating.cc
#ufloat_floating_tab_SOURCES      = ufloat-floating-tab.cc

asio_SOURCES			  = asio.cc
asio_LDADD = $(BOOST_SYSTEM_LIBS)  $(LDADD)
asio_LDFLAGS = $(BOOST_SYSTEM_LDFLAGS) $(PTHREAD_LDFLAGS) $(AM_LDFLAGS)
asio_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)

base64_SOURCES			  = base64.cc
cli_SOURCES                       = cli.cc
cstdlib_SOURCES                   = cstdlib.cc
deref_SOURCES                     = deref.cc
dirent_SOURCES                    = dirent.cc
erase_if_SOURCES                  = erase-if.cc
escape_SOURCES                    = escape.cc
fd_stream_SOURCES                 = fd-stream.cc
fifo_SOURCES                      = fifo.cc
finally_SOURCES                   = finally.cc
foreach_SOURCES                   = foreach.cc
has_if_SOURCES                    = has-if.cc
indent_SOURCES                    = indent.cc
intrusive_ptr_SOURCES             = intrusive-ptr.cc
intrusive_ptr_serialize_SOURCES   = intrusive-ptr-serialize.cc
path_SOURCES                      = path.cc
pid_file_SOURCES                  = pid-file.cc
semaphore_SOURCES                 = semaphore.cc
separator_SOURCES                 = separator.cc
singleton_ptr_SOURCES             = singleton-ptr.cc
statistics_SOURCES                = statistics.cc
symbol_SOURCES                    = symbol.cc
timer_SOURCES                     = timer.cc
ufloat_double_SOURCES             = ufloat-double.cc
unescape_SOURCES                  = unescape.cc
visitor_SOURCES                   = visitor.cc

EXTRA_DIST += ufloat-common.cc ufloat-config.h 666.txt

TESTS_ENVIRONMENT +=                                    \
  SRCDIR=$(srcdir)
