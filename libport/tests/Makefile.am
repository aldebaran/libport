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

# If your test programs includes header from the source tree:
AM_CPPFLAGS =		-I $(top_srcdir)		\
			-I $(top_srcdir)/src		\
			-I $(top_srcdir)/libport

# List your tests here:
TESTS =							\
			singleton1.test			\
			singleton2.test			\
			udouble1.test			\
			udoubletab1.test		\
			ufloat1.test			\
			ufloattab1.test			\
			ullfp1.test			\
			ullfptab1.test

config.h: $(top_srcdir)/src/config.h
	sed  -e '/URBI_UFLOAT/d' $(top_srcdir)/src/config.h > config.h

$(TESTS): config.h
	for i in $(TESTS); do \
          $(LN_S) -f $(top_srcdir)/build-aux/test.sh $$i || exit 1; \
        done

EXTRA_DIST = $(TESTS:.test=.stdout)
CLEANFILES = *.my_stdout *.my_stderr config.h
TESTS_ENVIRONMENT = SRCDIR=$(srcdir)

# List the tests that are expected to fail:
# XFAIL_TESTS =
