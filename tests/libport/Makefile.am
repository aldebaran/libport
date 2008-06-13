include $(top_srcdir)/flags.mk
include $(top_srcdir)/build-aux/check.mk

LAZY_TEST_SUITE = 1
# Program to check:
TESTS =				\
path				\
singleton-ptr			\
deref                      \
test-erase_if			\
test-escape                     \
test-has_if			\
test-unescape                   \
test-indent                     \
test-separator                  \
test-shared-ptr-intrusive       \
test-shared-ptr-non-intrusive   \
test-timer			\
visitor

%.log: %$(EXEEXT)
	@$(am__check_pre) $${dir}$< $(am__check_post)
TEST_LOGS = $(TESTS:=.log)

EXTRA_PROGRAMS = $(TESTS)
CLEANFILES += $(EXTRA_PROGRAMS)

#udouble1
#ufloat1
#ullfp1
#ullfptab1

AM_CPPFLAGS += $(BOOST_CPPFLAGS)
AM_LDFLAGS += $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
LDADD = $(top_builddir)/lib/libport/libport.la $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

path_SOURCES		= path.cc
singleton_ptr_SOURCES	= singleton-ptr.cc
deref_SOURCES      = deref.cc
test_erase_if_SOURCES	= test-erase_if.cc
test_escape_SOURCES     = test-escape.cc
test_has_if_SOURCES	= test-has_if.cc
test_unescape_SOURCES   = test-unescape.cc
test_indent_SOURCES     = test-indent.cc
test_separator_SOURCES  = test-separator.cc
test_shared_ptr_intrusive_SOURCES = test-shared-ptr-intrusive.cc
test_shared_ptr_non_intrusive_SOURCES = test-shared-ptr-non-intrusive.cc
test_timer_SOURCES      = test-timer.cc
#udouble1_SOURCES        = udouble1.cc
#ufloat1_SOURCES         = ufloat1.cc
#ullfp1_SOURCES          = ullfp1.cc
#ullfptab1_SOURCES       = ullfptab1.cc
visitor_SOURCES		= visitor.cc

# Automake thinks the following line is not POSIX, help it.
AUTOMAKE_OPTIONS += -Wno-portability

EXTRA_DIST += common-ufloat1.cc ufloat-config.h
TESTS_ENVIRONMENT = SRCDIR=$(srcdir)

