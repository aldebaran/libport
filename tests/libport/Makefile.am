include $(top_srcdir)/flags.mk

# Program to check:
check_PROGRAMS =		\
path				\
test-deref                      \
test-escape                     \
test-unescape                   \
test-indent                     \
test-separator                  \
test-shared-ptr                 \
test-timer			\
visitor

#udouble1
#ufloat1
#ullfp1
#ullfptab1

AM_CPPFLAGS += $(BOOST_CPPFLAGS)
AM_LDFLAGS += $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
LDADD = $(top_builddir)/lib/libport/libport.la $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

path_SOURCES		= path.cc
#singleton1_SOURCES      = singleton1.cc
#singleton2_SOURCES      = singleton2.cc
test_deref_SOURCES      = test-deref.cc
test_escape_SOURCES     = test-escape.cc
test_unescape_SOURCES   = test-unescape.cc
test_indent_SOURCES     = test-indent.cc
test_separator_SOURCES  = test-separator.cc
test_shared_ptr_SOURCES = test-shared-ptr.cc
test_timer_SOURCES      = test-timer.cc
#udouble1_SOURCES        = udouble1.cc
#ufloat1_SOURCES         = ufloat1.cc
#ullfp1_SOURCES          = ullfp1.cc
#ullfptab1_SOURCES       = ullfptab1.cc
visitor_SOURCES		= visitor.cc

# Automake thinks the following line is not POSIX, help it.
AUTOMAKE_OPTIONS += -Wno-portability
TESTS = $(check_PROGRAMS:$(EXEEXT)=.test)

$(TESTS):
	for i in $(TESTS); do \
          $(LN_S) -f $(top_builddir)/build-aux/test.sh $$i || exit 1; \
        done

EXTRA_DIST += $(TESTS:.test=.stdout) common-ufloat1.cc ufloat-config.h
CLEANFILES += $(TESTS:.test=.{my_stdout,my_stderr,valgrind.log})

TESTS_ENVIRONMENT = SRCDIR=$(srcdir)

# List the tests that are expected to fail:
# XFAIL_TESTS =
