##
## Makefile.am: This file is part of urbi-crypto.
## Copyright (C) Gostai S.A.S., 2006-2008.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.
## For comments, bug reports and feedback: http://www.urbiforge.com
##

include $(top_srcdir)/flags.mk
# include $(top_srcdir)/lib/urbi-crypto/urbi-crypto.mk

liburbicrypto_la = $(top_builddir)/lib/urbi-crypto/liburbicrypto.la

AM_CXXFLAGS += $(WARNING_CXXFLAGS)
AM_CPPFLAGS += 	-I$(top_srcdir) -I$(top_builddir)	\
		-I.

# Program to check:
check_PROGRAMS  =					\
			simple				\
			sign				\
			verify

simple_SOURCES  =	simple.cc
simple_LDADD    =	$(liburbicrypto_la) $(OPENSSL_LDFLAGS)
simple_CPPFLAGS = 	$(AM_CPPFLAGS)

sign_SOURCES    =	sign.cc
sign_LDADD      =	$(liburbicrypto_la) $(OPENSSL_LDFLAGS)
sign_CPPFLAGS   = 	$(AM_CPPFLAGS)

verify_SOURCES  =	verify.cc
verify_LDADD    =	$(liburbicrypto_la) $(OPENSSL_LDFLAGS)
verify_CPPFLAGS = 	$(AM_CPPFLAGS)

CLEANFILES += *.lib *.exp

# List your tests here:
TESTS =							\
			simple.test			\
			sign.test			\
			verify.test

$(TESTS):
	for i in $(TESTS); do \
          $(LN_S) -f $(top_builddir)/build-aux/test.sh $$i || exit 1; \
        done

EXTRA_DIST += $(TESTS:.test=.stdout)
CLEANFILES += $(TESTS:.test=.my_stdout)		\
	      $(TESTS:.test=.my_stderr)		\
	      $(TESTS:.test=.valgrind.log)

TESTS_ENVIRONMENT = SRCDIR=$(srcdir)

# List the tests that are expected to fail:
XFAIL_TESTS = sign.test verify.test
