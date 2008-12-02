include $(top_srcdir)/build-aux/init.mk

SUBDIRS = libport

# Program to check:
check_PROGRAMS = testme
nodist_check_SCRIPTS += $(top_builddir)/build-aux/test.sh
testme_SOURCES = testme.cc

# If your test programs includes header from the source tree:
# AM_CPPFLAGS += -I $(top_srcdir)/src
# Use AM_CFLAGS for C code. Don't play with CXXFLAGS/CFLAGS.

# If your test programs needs to link with some library from the source tree:
# LDADD = $(top_builddir)/src/libfoo.la

# List your tests here:
TESTS = testme.test

$(TESTS):
	for i in $(TESTS); do \
          $(LN_S) -f $(top_builddir)/build-aux/test.sh $$i || exit 1; \
        done

EXTRA_DIST += $(TESTS:.test=.stdout)
CLEANFILES += *.my_stdout *.my_stderr
TESTS_ENVIRONMENT += SRCDIR=$(srcdir)

$(top_builddir)/build-aux/test.sh: $(top_srcdir)/build-aux/test.sh.in
	cd $(top_builddir) && ./config.status build-aux/test.sh

# List the tests that are expected to fail:
# XFAIL_TESTS =
