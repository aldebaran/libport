include $(top_srcdir)/flags.mk

# No need to re-run the test if the sources have not changed.
LAZY_TEST_SUITE = 1
# For some reason, this rule *must* be defined before loading check.mk.
%.log: %$(EXEEXT)
	@$(am__check_pre) $${dir}$< $(am__check_post)
include $(top_srcdir)/build-aux/check.mk

# Program to check:
TESTS =						\
deref.cc					\
foreach.cc					\
path.cc						\
singleton-ptr.cc				\
test-erase_if.cc				\
test-escape.cc					\
test-finally.cc					\
test-has_if.cc					\
test-indent.cc					\
test-separator.cc				\
test-shared-ptr-intrusive.cc			\
test-shared-ptr-non-intrusive.cc		\
test-shared-ptr-serialize.cc			\
test-symbol-serialize.cc			\
test-timer.cc					\
test-unescape.cc				\
visitor.cc

#udouble1
#ufloat1
#ullfp1
#ullfptab1

TEST_LOGS = $(TESTS:.cc=.log)

EXTRA_PROGRAMS = $(TESTS:.cc=)
CLEANFILES += $(EXTRA_PROGRAMS)

AM_CPPFLAGS += $(BOOST_CPPFLAGS)
AM_LDFLAGS += $(BOOST_SERIALIZATION_LDFLAGS) $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
LDADD = $(top_builddir)/lib/libport/libport.la \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIBS) $(BOOST_SERIALIZATION_LIBS)

#udouble1_SOURCES        = udouble1.cc
#ufloat1_SOURCES         = ufloat1.cc
#ullfp1_SOURCES          = ullfp1.cc
#ullfptab1_SOURCES       = ullfptab1.cc
deref_SOURCES      	= deref.cc
foreach_SOURCES      	= foreach.cc
path_SOURCES		= path.cc
singleton_ptr_SOURCES	= singleton-ptr.cc
test_erase_if_SOURCES	= test-erase_if.cc
test_escape_SOURCES     = test-escape.cc
test_finally_SOURCES	= test-finally.cc
test_has_if_SOURCES	= test-has_if.cc
test_indent_SOURCES     = test-indent.cc
test_separator_SOURCES  = test-separator.cc
test_shared_ptr_intrusive_SOURCES = test-shared-ptr-intrusive.cc
test_shared_ptr_non_intrusive_SOURCES = test-shared-ptr-non-intrusive.cc
test_shared_ptr_serialize_SOURCES = test-shared-ptr-serialize.cc
test_symbol_serialize_SOURCES = test-symbol-serialize.cc
test_timer_SOURCES      = test-timer.cc
test_unescape_SOURCES   = test-unescape.cc
visitor_SOURCES		= visitor.cc

# Automake thinks the following line is not POSIX, help it.
AUTOMAKE_OPTIONS += -Wno-portability

EXTRA_DIST += common-ufloat1.cc ufloat-config.h
TESTS_ENVIRONMENT = SRCDIR=$(srcdir)

