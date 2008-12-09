include $(top_srcdir)/flags.mk
include $(top_srcdir)/build-aux/m4sh.mk
include $(top_srcdir)/build-aux/wine-ld-library-path.mk

# No need to re-run the test if the sources have not changed.
LAZY_TEST_SUITE = 1

# For some reason, this rule *must* be defined before loading
# check.mk.
#
# This is really weird: libtool does not create a wrapper with EXEEXT,
# but this extension is required here, since the rules do mention the
# EXEEXT.  So we have to remove the extension from $<.
%.log: %$(EXEEXT)
	@$(am__check_pre) ./$(<:$(EXEEXT)=) $(am__check_post)
include $(top_srcdir)/build-aux/check.mk

# Program to check:
TESTS =                                        \
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
  semaphore.cc                                  \
  separator.cc                                  \
  intrusive-ptr.cc                              \
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

cli_SOURCES                       = cli.cc
cstdlib_SOURCES                   = cstdlib.cc
deref_SOURCES                     = deref.cc
dirent_SOURCES                    = dirent.cc
fd_stream_SOURCES                 = fd-stream.cc
foreach_SOURCES                   = foreach.cc
path_SOURCES                      = path.cc
singleton_ptr_SOURCES             = singleton-ptr.cc
symbol_SOURCES                    = symbol.cc
erase_if_SOURCES                  = erase-if.cc
escape_SOURCES                    = escape.cc
fifo_SOURCES                      = fifo.cc
finally_SOURCES                   = finally.cc
has_if_SOURCES                    = has-if.cc
indent_SOURCES                    = indent.cc
semaphore_SOURCES                 = semaphore.cc
separator_SOURCES                 = separator.cc
intrusive_ptr_SOURCES             = intrusive-ptr.cc
intrusive_ptr_serialize_SOURCES   = intrusive-ptr-serialize.cc
statistics_SOURCES                = statistics.cc
timer_SOURCES                     = timer.cc
unescape_SOURCES                  = unescape.cc
#ufloat_double_tab_SOURCES        = ufloat-double-tab.cc
ufloat_double_SOURCES             = ufloat-double.cc
#ufloat_float_tab_SOURCES         = ufloat-float-tab.cc
#ufloat_float_SOURCES             = ufloat-float.cc
#ufloat_floating_tab_SOURCES      = ufloat-floating-tab.cc
#ufloat_floating_SOURCES          = ufloat-floating.cc
visitor_SOURCES                   = visitor.cc

EXTRA_DIST += ufloat-common.cc ufloat-config.h 666.txt

WINEPREFIX_ENVIRONMENT :=			\
  $(shell					\
     $(wine_ld_library_path)			\
     --to=$(abs_builddir)			\
     $(abs_top_builddir)/lib/libport)

# See above for an explanation on the $(EXEEXT) business.
TESTS_ENVIRONMENT +=                                    \
  SRCDIR=$(srcdir)                                      \
  $(WINEPREFIX_ENVIRONMENT)


CLEANFILES += $(EXTRA_PROGRAMS:$(EXEEXT)=)
