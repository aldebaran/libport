## Vaucanson, a generic library for finite state machines.
## Copyright (C) 2006, 2007 The Vaucanson Group.
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

# Restructured Text section and subsection.
am__rst_section =    sed 'p;s/./=/g;p;g'
am__rst_subsection = sed 'p;s/./-/g;p;g'

# Put in a box.
am__text_box = $(AWK) '{gsub ("\\.  ", "\n"); print $$0; }' |	\
$(AWK) '							\
max < length($$0) {						\
  final= final (final ? "\n" : "") " " $$0;			\
  max = length($$0);						\
}								\
END { 								\
  for (i = 0; i < max + 2 ; ++i)				\
    line = line "=";						\
  print line;							\
  print final;							\
  print line;							\
}'

# To be inserted before the command running the test.  Stores in $dir
# the directory containing $<, and passes the TEST_ENVIRONMENT.
am__check_pre =					\
if test -f ./$<; then dir=./;			\
elif test -f $<; then dir=;			\
else dir="$(srcdir)/"; fi;			\
$(TESTS_ENVIRONMENT)

# To be appended to the command running the test.  Handles the stdout
# and stderr redirection, and catch the exit status.
am__check_post =					\
>$@-t 2>&1;						\
estatus=$$?;						\
case $$estatus:" $(XFAIL_TESTS) " in			\
    0:*" $$(basename $<) "*) res='XPASS';;		\
    0:*)        res='PASS' ;;				\
    77:*)       res='SKIP' ;;				\
    *:*" $$(basename $<) "*) res='XFAIL';;		\
    *:*)        res='FAIL' ;;				\
   esac;						\
echo "$$res: $$(basename $<)";				\
echo "$$res: $$(basename $<) (exit: $$estatus)" |	\
  $(am__rst_subsection) >$@;				\
cat $@-t >>$@;						\
rm $@-t

SUFFIXES = .log .test
# From a test file to a log file.
# Do not use a regular `.test.log:' rule here, since in that case the
# following rule (without incoming extension) will mask this one.
%.log: %.test
	@$(am__check_pre) $${dir}$< $(am__check_post)

# The exact same commands, but for programs without extensions.
%.log: %
	@$(am__check_pre) $${dir}$< $(am__check_post)

TEST_LOGS ?= $(TESTS:.test=.log)
TEST_SUITE_LOG = test-suite.log

$(TEST_SUITE_LOG): $(TEST_LOGS)
	@results=$$(for f in $(TEST_LOGS); do sed 1q $$f; done);	\
	all=$$(echo "$$results" | wc -l | sed -e 's/^[ \t]*//');	\
	fail=$$(echo "$$results" | grep -c '^FAIL');			\
	pass=$$(echo "$$results" | grep -c '^PASS');			\
	skip=$$(echo "$$results" | grep -c '^SKIP');			\
	xfail=$$(echo "$$results" | grep -c '^XFAIL');			\
	xpass=$$(echo "$$results" | grep -c '^XPASS');			\
	case fail=$$fail:xfail=$$xfail:xpass=$$xpass in			\
	  fail=0:xfail=0:xpass=*)					\
	    msg="All $$all tests passed.  ";;				\
	  fail=0:xfail=*:xpass=*)					\
	    msg="All $$all tests behaved as expected";			\
	    msg="$$msg ($$xfail expected failures).  ";;		\
	  fail=*:xfail=*:xpass=0)					\
	    msg="$$fail of $$all tests failed.  ";;			\
	  fail=*:xfail=*:xpass=*)					\
	    msg="$$fail of $$all tests did not behave as expected";	\
	    msg="$$msg ($$xpass unexpected passes).  ";;		\
	  *)								\
            echo >&2 "incorrect case"; exit 4;;				\
	esac;								\
	if test "$$skip" -ne 0; then					\
	  msg="$$msg($$skip tests were not run).  ";			\
	fi;								\
	if test "$$fail" -ne 0; then					\
	  {								\
	    echo "$(PACKAGE_STRING): $(subdir)/$(TEST_SUITE_LOG)" |	\
	      $(am__rst_section);					\
	    echo;							\
	    echo "$$msg";						\
	    echo;							\
	    echo ".. contents:: :depth: 2";				\
	    echo;							\
	    for f in $(TEST_LOGS);					\
	    do								\
	      case $$(sed 1q $$f) in					\
	        SKIP:*|PASS:*|XFAIL:*);;				\
	        *) echo; cat $$f;;					\
	      esac;							\
	    done;							\
	  } >$(TEST_SUITE_LOG).tmp;					\
	  mv $(TEST_SUITE_LOG).tmp $(TEST_SUITE_LOG);			\
	  msg="$${msg}See $(subdir)/$(TEST_SUITE_LOG).  ";		\
	  if test -n "$(PACKAGE_BUGREPORT)"; then			\
	    msg="$${msg}Please report it to $(PACKAGE_BUGREPORT).  ";	\
	  fi;								\
	fi;								\
	echo "$$msg" | $(am__text_box);					\
	if test x"$$VERBOSE" != x && test "$$fail" -ne 0; then		\
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
