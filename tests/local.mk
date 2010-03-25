# No need to re-run the test if the sources have not changed.
LAZY_TEST_SUITE = 1
STRICT_TEST_LOGS = $(shell $(LIST_FAILED_TEST_LOGS))

# Program to check:
TESTS += $(TESTS_BINARIES)

include tests/bin/local.mk
include tests/libport/local.mk
include tests/sched/local.mk
include tests/serialize/local.mk

# We disable this framework because it is not usable yet.
# It has interesting code, this is why it is not thrown away.
#
# Program to check:
# check_PROGRAMS = testme
# nodist_check_SCRIPTS += $(top_builddir)/build-aux/test.sh
# testme_SOURCES = testme.cc

# If your test programs includes header from the source tree:
# AM_CPPFLAGS += -I $(top_srcdir)/src
# Use AM_CFLAGS for C code. Don't play with CXXFLAGS/CFLAGS.

# If your test programs needs to link with some library from the source tree:
# LDADD = $(top_builddir)/src/libfoo.la

# List your tests here:
#TESTS += testme.test

#$(TESTS):
#	for i in $(TESTS); do \
#          $(LN_S) -f $(top_builddir)/build-aux/test.sh $$i || exit 1; \
#        done

#EXTRA_DIST += $(TESTS:.test=.stdout)
#CLEANFILES += *.my_stdout *.my_stderr
#TESTS_ENVIRONMENT += SRCDIR=$(srcdir)

#$(top_builddir)/build-aux/test.sh: $(top_srcdir)/build-aux/test.sh.in
#	cd $(top_builddir) && ./config.status build-aux/test.sh
