include $(top_srcdir)/libport/build-aux/init.mk

# Program to check:
check_PROGRAMS =		\
singleton1			\
singleton2			\
test-deref			\
test-escape			\
test-indent			\
test-shared-ptr			\
test-separator			\
udouble1			\
ufloat1				\
ullfp1				\
ullfptab1

LDADD = $(top_builddir)/libport/libport.la

singleton1_SOURCES =	singleton1.cc
singleton2_SOURCES =	singleton2.cc
test_deref_SOURCES =    test-deref.cc
test_escape_SOURCES =   test-escape.cc
test_indent_SOURCES =   test-indent.cc
test_separator_SOURCES =test-separator.cc
test_shared_ptr_SOURCES =test-shared-ptr.cc
udouble1_SOURCES =	udouble1.cc
ufloat1_SOURCES =	ufloat1.cc
ullfp1_SOURCES =	ullfp1.cc
ullfptab1_SOURCES =	ullfptab1.cc

# Automake thinks the following line is not POSIX, help it.
AUTOMAKE_OPTIONS += -Wno-portability
TESTS = $(check_PROGRAMS:$(EXEEXT)=.test)

## ---------------- ##
## libport/config.h ##
## ---------------- ##

# Create a false libport/config.h just for tests, which
# does not specify the sort of ufloats to use.
BUILT_SOURCES += libport/config.h
# Do not define the variety of ufloats to use.
libport/config.h: $(top_builddir)/libport/include/libport/config.h
	test -d libport || mkdir libport
	{							\
	  echo "#define LIBPORT_TEST 1";			\
	  sed -e '/# *define.*URBI_UFLOAT.*/s,,/* & */,' $<;	\
	} > $@.t
	mv $@.t $@
clean-local:
	rm -rf libport

# Find the libport files, but our config.h for tests first.
AM_CPPFLAGS += -I. -I$(top_srcdir) $(BOOST_CPPFLAGS)

$(TESTS):
	for i in $(TESTS); do \
          $(LN_S) -f $(top_builddir)/libport/build-aux/test.sh $$i || exit 1; \
        done

EXTRA_DIST += $(TESTS:.test=.stdout) common_ufloat1.cc
CLEANFILES += $(TESTS:.test=.{my_stdout,my_stderr,valgrind.log})

TESTS_ENVIRONMENT = SRCDIR=$(srcdir)

# List the tests that are expected to fail:
# XFAIL_TESTS =
