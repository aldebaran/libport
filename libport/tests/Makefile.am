# Program to check:
check_PROGRAMS =					\
			singleton1			\
			singleton2			\
			udouble1			\
			ufloat1				\
			ullfp1				\
			ullfptab1

singleton1_SOURCES =	singleton1.cc
singleton2_SOURCES =	singleton2.cc
udouble1_SOURCES =	udouble1.cc
ufloat1_SOURCES =	ufloat1.cc
ullfp1_SOURCES =	ullfp1.cc
ullfptab1_SOURCES =	ullfptab1.cc

# List your tests here:
TESTS =							\
			singleton1.test			\
			singleton2.test			\
			udouble1.test			\
			ufloat1.test			\
			ullfp1.test			\
			ullfptab1.test

## ---------------- ##
## libport/config.h ##
## ---------------- ##

# Create a false libport/config.h just for tests, which
# does not specify the sort of ufloats to use.
BUILT_SOURCES = libport/config.h
# Do not define the variety of ufloats to use.
libport/config.h: $(top_builddir)/libport/config.h
	test -d libport || mkdir libport
	{							\
	  echo "#define LIBPORT_TEST 1";			\
	  sed -e '/# *define.*URBI_UFLOAT.*/s,,/* & */,' $<;	\
	} > $@.t
	mv $@.t $@
clean-local:
	rm -rf libport

# Find the libport files, but our config.h for tests first.
AM_CPPFLAGS = -I. -I$(top_srcdir)




$(TESTS):
	for i in $(TESTS); do \
          $(LN_S) -f $(top_builddir)/build-aux/test.sh $$i || exit 1; \
        done

EXTRA_DIST = $(TESTS:.test=.stdout) common_ufloat1.cc
CLEANFILES = *.my_stdout *.my_stderr

TESTS_ENVIRONMENT = SRCDIR=$(srcdir)

# List the tests that are expected to fail:
# XFAIL_TESTS =
