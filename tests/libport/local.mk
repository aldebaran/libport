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

# Program to check:
TESTS +=					\
  libport/asio.cc				\
  libport/base64.cc				\
  libport/cli.cc				\
  libport/cstdlib.cc				\
  libport/deref.cc				\
  libport/dirent.cc				\
  libport/fd-stream.cc				\
  libport/foreach.cc				\
  libport/path.cc				\
  libport/singleton-ptr.cc			\
  libport/symbol.cc				\
  libport/erase-if.cc				\
  libport/escape.cc				\
  libport/fifo.cc				\
  libport/finally.cc				\
  libport/has-if.cc				\
  libport/indent.cc				\
  libport/intrusive-ptr.cc			\
  libport/semaphore.cc				\
  libport/separator.cc				\
  libport/pid-file.cc				\
  libport/statistics.cc				\
  libport/timer.cc				\
  libport/unescape.cc				\
  libport/ufloat-double.cc			\
  libport/visitor.cc

# This test has been commented out as we do not support ufloat /= double.
# ufloat-float.cc

if WITH_BOOST_SERIALIZATION
TESTS += libport/intrusive-ptr-serialize.cc
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

libport_asio_SOURCES			  = libport/asio.cc
libport_asio_LDADD = $(BOOST_SYSTEM_LIBS)  $(LDADD)
libport_asio_LDFLAGS = $(BOOST_SYSTEM_LDFLAGS) $(PTHREAD_LDFLAGS) $(AM_LDFLAGS)
libport_asio_CXXFLAGS = $(PTHREAD_CFLAGS) $(AM_CXXFLAGS)

libport_base64_SOURCES                    = libport/base64.cc
libport_cli_SOURCES                       = libport/cli.cc
libport_cstdlib_SOURCES                   = libport/cstdlib.cc
libport_deref_SOURCES                     = libport/deref.cc
libport_dirent_SOURCES                    = libport/dirent.cc
libport_erase_if_SOURCES                  = libport/erase-if.cc
libport_escape_SOURCES                    = libport/escape.cc
libport_fd_stream_SOURCES                 = libport/fd-stream.cc
libport_fifo_SOURCES                      = libport/fifo.cc
libport_finally_SOURCES                   = libport/finally.cc
libport_foreach_SOURCES                   = libport/foreach.cc
libport_has_if_SOURCES                    = libport/has-if.cc
libport_indent_SOURCES                    = libport/indent.cc
libport_intrusive_ptr_SOURCES             = libport/intrusive-ptr.cc
libport_intrusive_ptr_serialize_SOURCES   = libport/intrusive-ptr-serialize.cc
libport_path_SOURCES                      = libport/path.cc
libport_pid_file_SOURCES                  = libport/pid-file.cc
libport_semaphore_SOURCES                 = libport/semaphore.cc
libport_separator_SOURCES                 = libport/separator.cc
libport_singleton_ptr_SOURCES             = libport/singleton-ptr.cc
libport_statistics_SOURCES                = libport/statistics.cc
libport_symbol_SOURCES                    = libport/symbol.cc
libport_timer_SOURCES                     = libport/timer.cc
libport_ufloat_double_SOURCES             = libport/ufloat-double.cc
libport_unescape_SOURCES                  = libport/unescape.cc
libport_visitor_SOURCES                   = libport/visitor.cc

EXTRA_DIST += libport/ufloat-common.cc libport/ufloat-config.h libport/666.txt

TESTS_ENVIRONMENT +=				\
  SRCDIR=$(srcdir)