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
  path.cc					\
  singleton-ptr.cc				\
  symbol.cc					\
  erase_if.cc					\
  escape.cc					\
  fifo.cc					\
  finally.cc					\
  has_if.cc					\
  indent.cc					\
  separator.cc					\
  shared-ptr-intrusive.cc			\
  shared-ptr-non-intrusive.cc			\
  shared-ptr-serialize.cc			\
  statistics.cc					\
  timer.cc					\
  unescape.cc					\
  ufloat-double.cc				\
  ufloat-float.cc				\
  visitor.cc

# Tabulation does not seem to work: doesn't even compile.
#ufloat-float-tab.cc				\
#ufloat-double-tab.cc				\
#ufloat-floating-tab.cc
#ufloat-floating.cc

TEST_LOGS = $(TESTS:.cc=.log)

EXTRA_PROGRAMS = $(TESTS:.cc=)
CLEANFILES += $(EXTRA_PROGRAMS)

AM_CPPFLAGS +=					\
  $(BOOST_CPPFLAGS)				\
  -DBUILDING_LIBPORT
AM_LDFLAGS +=					\
  $(BOOST_SERIALIZATION_LDFLAGS) 		\
  $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
LDADD +=					\
  $(top_builddir)/lib/libport/libport.la	\
  $(BOOST_SERIALIZATION_LIBS)			\
  $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

deref_SOURCES				= deref.cc
foreach_SOURCES				= foreach.cc
path_SOURCES				= path.cc
singleton_ptr_SOURCES			= singleton-ptr.cc
symbol_SOURCES				= symbol.cc
erase_if_SOURCES			= erase_if.cc
escape_SOURCES				= escape.cc
fifo_SOURCES				= fifo.cc
finally_SOURCES				= finally.cc
has_if_SOURCES				= has_if.cc
indent_SOURCES				= indent.cc
separator_SOURCES			= separator.cc
shared_ptr_intrusive_SOURCES		= shared-ptr-intrusive.cc
shared_ptr_non_intrusive_SOURCES	= shared-ptr-non-intrusive.cc
shared_ptr_serialize_SOURCES		= shared-ptr-serialize.cc
statistics_SOURCES			= statistics.cc
timer_SOURCES				= timer.cc
unescape_SOURCES			= unescape.cc
#ufloat_double_tab_SOURCES		= ufloat-double-tab.cc
ufloat_double_SOURCES			= ufloat-double.cc
#ufloat_float_tab_SOURCES		= ufloat-float-tab.cc
ufloat_float_SOURCES			= ufloat-float.cc
#ufloat_floating_tab_SOURCES		= ufloat-floating-tab.cc
#ufloat_floating_SOURCES		= ufloat-floating.cc
visitor_SOURCES				= visitor.cc

EXTRA_DIST += ufloat-common.cc ufloat-config.h
TESTS_ENVIRONMENT = SRCDIR=$(srcdir)
