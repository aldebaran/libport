## Vaucanson, a generic library for finite state machines.
## Copyright (C) 2006 The Vaucanson Group.
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## The complete GNU General Public Licence Notice can be found as the
## `COPYING' file in the root directory.
##
## The Vaucanson Group consists of people listed in the `AUTHORS' file.

## Override the definition from Automake to generate a log file with
## failed tests.  It also supports parallel make checks.
## 
## This file provides special support for "unit tests", that is to
## say, tests that (once run) no longer need to be re-compiled and
## re-run at each "make check", unless their sources changed.  To
## enable unit-test supports, define LAZY_TEST_SUITE.  In such a
## setting, that heavily relies on correct dependencies, its users may
## prefer to define EXTRA_PROGRAMS instead of check_PROGRAMS, because
## it allows intertwined compilation and execution of the tests.
## Sometimes this helps catching errors earlier (you don't have to
## wait for all the tests to be compiled).
##
## Define TEST_SUITE_LOG to be the name of the global log to create.
## Define TEST_LOGS to the set of logs to include in it.  It defaults
## to $(TESTS:.test=.log).

## We use GNU Make extensions, and override check-TESTS.
AUTOMAKE_OPTIONS += -Wno-portability -Wno-override

SUFFIXES = .log .test
# From a test file to a log file.
# Do not use a regular `.test.log:' rule here, since in that case the
# following rule (without incoming extension) will mask this one.
%.log: %.test
	@if test -f ./$<; then dir=./;			\
	elif test -f $<; then dir=;			\
	else dir="$(srcdir)/"; fi;			\
	$(TESTS_ENVIRONMENT) $${dir}$< >$@-t 2>&1;	\
	status=$$?;					\
	case $$status:" $(XFAIL_TESTS) " in		\
	    0:*' $< '*) res='XPASS';;			\
	    0:*)        res='PASS' ;;			\
	    77:*)       res='SKIP' ;;			\
	    *:*' $< '*) res='XFAIL';;			\
	    *:*)        res='FAIL' ;;			\
	   esac;					\
	echo "$$res: $$(basename $<)";			\
	echo "$$res: $$(basename $<) (exit: $$status)" >$@
	@cat $@-t >>$@
	@rm $@-t

# The exact same commands, but for programs without extensions.
%.log: %
	@if test -f ./$<; then dir=./;			\
	elif test -f $<; then dir=;			\
	else dir="$(srcdir)/"; fi;			\
	$(TESTS_ENVIRONMENT) $${dir}$< >$@-t 2>&1;	\
	status=$$?;					\
	case $$status:" $(XFAIL_TESTS) " in		\
	    0:*' $< '*) res='XPASS';;			\
	    0:*)        res='PASS' ;;			\
	    77:*)       res='SKIP' ;;			\
	    *:*' $< '*) res='XFAIL';;			\
	    *:*)        res='FAIL' ;;			\
	   esac;					\
	echo "$$res: $$(basename $<)";			\
	echo "$$res: $$(basename $<) (exit: $$status)" >$@
	@cat $@-t >>$@
	@rm $@-t

TEST_LOGS ?= $(TESTS:.test=.log)
TEST_SUITE_LOG = test-suite.log

$(TEST_SUITE_LOG): $(TEST_LOGS)
	@for f in $(TEST_LOGS);						\
	do								\
	  case $$(sed 1q $$f) in					\
	    SKIP:*|PASS:*|XFAIL:*);;					\
	    *) cat $$f;							\
	  esac;								\
	done >$(TEST_SUITE_LOG);					\
	results=$$(for f in $(TEST_LOGS); do sed 1q $$f; done);		\
	all=$$(echo "$$results" | wc -l | sed -e 's/^[ \t]*//');	\
	fail=$$(echo "$$results" | grep -c '^FAIL');			\
	pass=$$(echo "$$results" | grep -c '^PASS');			\
	skip=$$(echo "$$results" | grep -c '^SKIP');			\
	xfail=$$(echo "$$results" | grep -c '^XFAIL');			\
	xpass=$$(echo "$$results" | grep -c '^XPASS');			\
	case fail=$$fail:xfail=$$xfail:xpass=$$xpass in			\
	  fail=0:xfail=0:xpass=*)					\
	    msg="All $$all tests passed";;				\
	  fail=0:xfail=*:xpass=*)					\
	    msg="All $$all tests behaved as expected";			\
	    msg="$$msg ($$xfail expected failures)";;			\
	  fail=*:xfail=*:xpass=0)					\
	    msg="$$fail of $$all tests failed";;			\
	  fail=*:xfail=*:xpass=*)					\
	    msg="$$fail of $$all tests did not behave as expected";	\
	    msg="$$msg ($$xpass unexpected passes)";;			\
	  *)								\
            echo >&2 "incorrect case"; exit 4;;				\
	esac;								\
	{								\
	  echo "$$msg";							\
	  if test "$$skip" -ne 0; then					\
	    echo "($$skip tests were not run)";				\
	  fi;								\
	  if test "$$fail" -ne 0; then					\
	    echo "See $(subdir)/$(TEST_SUITE_LOG)";			\
	    if test -n "$(PACKAGE_BUGREPORT)"; then			\
	      echo "Please report it to $(PACKAGE_BUGREPORT)";		\
	    fi;								\
	  fi;								\
	} | sed -e '1h;1s/./=/g;1p;1x;$$p;$$x';				\
	if test x"$$VERBOSE" != x; then					\
	  cat $(TEST_SUITE_LOG);					\
	fi;								\
	test "$$fail" -eq 0

# Run all the tests.
check-TESTS:
	@if test -z '$(LAZY_TEST_SUITE)'; then	\
	  rm -f $(TEST_SUITE_LOG) $(TEST_LOGS);	\
	fi
	@$(MAKE) $(TEST_SUITE_LOG)

clean-check-TESTS:
	rm -f $(TEST_SUITE_LOG) $(TEST_LOGS)
.PHONY: clean-check-TESTS
clean-local: clean-check-TESTS
