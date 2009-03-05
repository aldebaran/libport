## Vaucanson, a generic library for finite state machines.
## Copyright (C) 2006, 2007, 2008, 2009 The Vaucanson Group.
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
## If some of the tests are not lazy, define STRICT_TEST_LOGS as the
## set of TEST_LOGS that must be cleaned before rerunning the tests.
##
## Define TEST_SUITE_LOG to be the name of the global log to create.
## Define TEST_LOGS to the set of logs to include in it.  It defaults
## to $(TESTS:.test=.log).
##
## In addition to the magic "exit 77 means SKIP" feature (which was
## imported from automake), there is a magic "exit 177 means FAIL" feature
## which is useful if you need to issue a hard error no matter whether the
## test is XFAIL or not.

# Set this to `false' to disable hard errors.
ENABLE_HARD_ERRORS ?= :

## We use GNU Make extensions (%-rules), and override check-TESTS.
AUTOMAKE_OPTIONS += -Wno-portability -Wno-override

# Restructured Text title and section.
am__rst_title   = sed 's/.*/   &   /;h;s/./=/g;p;x;p;g;p;s/.*//'
am__rst_section = sed 'p;s/./=/g;p;g'
am__rst_pre     = sed 's/^/	/;$${p;g;}'
am__rst_block   = sed 's/.*/&::/;$${p;g;}'

# Put stdin (possibly several lines separated by ".  ") in a 123-box.
am__text_box =					\
$(AWK) '					\
{						\
  n = split($$0, lines, "\\.  ");		\
  max = 0;					\
  for (i = 1; i <= n; ++i)			\
    if (max < length(lines[i]))			\
      max = length(lines[i]);			\
  ++max;					\
  for (i = 0; i < max + 6 ; ++i)		\
    line = line "-";				\
  format="|   %-" max "s   |\n";		\
  print "." line ".";				\
  for (i = 1; i <= n; ++i)			\
    if (lines[i])				\
      printf format, lines[i] ".";		\
  print "`" line "'"'"'";			\
}'

# Output stdin converting ".  " as ".\n".
am__rst_bullet = 				\
$(AWK) '					\
{						\
  n = split($$0, lines, "\\.  ");		\
  for (i = 0; i <= n ; ++i)			\
    if (lines[i])				\
      print lines[i] ".";			\
  print "";					\
}'


# If stdout is a non-dumb tty, use colors.  If test -t is not supported,
# then this fails; a conservative approach.  Of course do not redirect
# stdout here, just stderr.  Finish by sgr0 to help "set -x" debugging
# return to standard display.
am__tty_colors =				\
blu=;						\
grn=;						\
lgn=;						\
red=;						\
std=;						\
ylw=;						\
test "X$(AM_COLOR_TESTS)" != Xno &&		\
test "X$$TERM" != Xdumb &&			\
{						\
  test "X$(AM_COLOR_TESTS)" = Xalways ||	\
  {						\
    test -t 1 2>/dev/null &&			\
    tput bold 1 >/dev/null 2>&1 &&		\
    tput setaf 1 >/dev/null 2>&1 &&		\
    tput sgr0 >/dev/null 2>&1;			\
  }						\
} &&						\
{						\
  blu=$$(tput setaf 4);				\
  grn=$$(tput setaf 2);				\
  lgn=$$(tput bold)$$(tput setaf 2);		\
  red=$$(tput setaf 1);				\
  ylw=$$(tput setaf 3);				\
  std=$$(tput sgr0);				\
}

# To be inserted before the command running the test.  Creates the
# directory for the log if needed.  Stores in $dir the directory
# containing $<, and passes the TESTS_ENVIRONMENT.
am__check_pre =					\
$(mkdir_p) "$$(dirname $@)";			\
if test -f ./$<; then dir=./;			\
elif test -f $<; then dir=;			\
else dir="$(srcdir)/"; fi;			\
$(TESTS_ENVIRONMENT)

# The exit status denoting a test to skip.
SKIP_STATUS ?= 77
# The exit status denoting a hard error, that cannot be saved by XFAIL.
HARD_STATUS ?= 177

# To be appended to the command running the test.  Handles the stdout
# and stderr redirection, and catch the exit status.
am__check_post =				\
>$@-t 2>&1;					\
estatus=$$?;					\
if test $$estatus -eq 177; then			\
  $(ENABLE_HARD_ERRORS) || estatus=1;		\
fi;						\
$(am__tty_colors);				\
xfailed=PASS;					\
for xfail in : $(XFAIL_TESTS); do		\
  case $< in					\
    $$xfail | */$$xfail) xfailed=XFAIL; break;;	\
  esac;						\
done;						\
for tfail in : $(TFAIL_TESTS); do		\
  case $< in					\
    $$tfail | */$$tfail) xfailed=TFAIL; break;;	\
  esac;						\
done;						\
case $$estatus:$$xfailed in			\
    0:?FAIL)          col=$$red; res=XPASS;;	\
    0:*)              col=$$grn; res=PASS ;;	\
    $(SKIP_STATUS):*) col=$$blu; res=SKIP ;;	\
    $(HARD_STATUS):*) col=$$red; res=FAIL ;;	\
    *:TFAIL)          col=$$ylw; res=TFAIL;;	\
    *:XFAIL)          col=$$lgn; res=XFAIL;;	\
    *:*)              col=$$red; res=FAIL ;;	\
esac;						\
echo "$${col}$$res$${std}: $@";			\
echo "$$res: $@ (exit: $$estatus)" |		\
  $(am__rst_section) >$@;			\
cat $@-t >>$@;					\
rm $@-t

# From a test file to a log file.
# Do not use a regular `.test.log:' rule here, since in that case the
# following rule (without incoming extension) will mask this one.
%.log: %.test $(check_programs)
	@$(am__check_pre) $${dir}$< $(am__check_post)

# The exact same commands, but for programs.
#
# Should be active by default, because it sometimes triggers when in
# should not.  For instance I had foo.chk tests that relied on
# directories with the name, without extensions (foo).  Then Make
# tried to run the directories to produce foo.log, not foo.chk.
#
#%.log: %$(EXEEXT)
#	@$(am__check_pre) $${dir}$< $(am__check_post)

TEST_LOGS ?= $(TESTS:.test=.log)
TEST_SUITE_LOG ?= test-suite.log

check_programs =							\
  $(nodist_check_SCRIPTS)  $(dist_check_SCRIPTS)  $(check_SCRIPTS)	\
  $(nodist_check_PROGRAMS) $(dist_check_PROGRAMS) $(check_PROGRAMS)

$(TEST_SUITE_LOG): $(TEST_LOGS)
	@results=$$(for f in $(TEST_LOGS); do sed 1q $$f; done);	\
	all=$$(  echo "$$results" | grep -c '^.');			\
	fail=$$( echo "$$results" | grep -c '^FAIL');			\
	pass=$$( echo "$$results" | grep -c '^PASS');			\
	skip=$$( echo "$$results" | grep -c '^SKIP');			\
	tfail=$$(echo "$$results" | grep -c '^TFAIL');			\
	xfail=$$(echo "$$results" | grep -c '^XFAIL');			\
	xpass=$$(echo "$$results" | grep -c '^XPASS');			\
	case fail=$$fail:xpass=$$xpass in				\
	  fail=0:xpass=0)						\
	    msg="The test suite passed.  ";				\
	    exit=true;;							\
	  *)								\
	    msg="The test suite failed.  ";				\
	    exit=false;;						\
	esac;								\
	msg="$$msg   - $$pass/$$all tests passed.  ";			\
	test $$fail -eq 0 ||						\
	  msg="$$msg   - $$fail failures.  ";				\
	test $$xpass -eq 0 ||						\
	  msg="$$msg   - $$xpass unexpected pass.  ";			\
	test $$xfail -eq 0 ||						\
	  msg="$$msg   - $$xfail expected failures.  ";			\
	test $$tfail -eq 0 ||						\
	  msg="$$msg   - $$tfail expected temporary failures.  ";	\
	test $$skip -eq 0 ||						\
	  msg="$$msg   - $$skip skipped tests.  ";			\
	if test "$$exit" = false; then					\
	  {								\
	    echo "$(PACKAGE_STRING): $(subdir)/$(TEST_SUITE_LOG)" |	\
	      $(am__rst_title);						\
	    echo "$$msg" | $(am__rst_bullet);				\
	    echo "TESTS_ENVIRONMENT" | $(am__rst_block);		\
	    echo "$(TESTS_ENVIRONMENT)" | 				\
	       tr ' ' '\n' | 						\
	       sort |							\
               $(am__rst_pre);						\
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
	$(am__tty_colors);						\
	if $$exit; then echo $$grn; else echo $$red; fi;		\
	  echo "$$msg" | $(am__text_box);				\
	echo $$std;							\
	test x"$$VERBOSE" = x || $$exit || cat $(TEST_SUITE_LOG);	\
	$$exit

# Run all the tests.
check-TESTS:
	@if test -n '$(LAZY_TEST_SUITE)'; then	\
	  rm -f $(STRICT_TEST_LOGS);		\
	else					\
	  rm -f $(TEST_LOGS);			\
	fi
	@rm -f $(TEST_SUITE_LOG)
	@$(MAKE) $(TEST_SUITE_LOG)


## -------------- ##
## Produce HTML.  ##
## -------------- ##

TEST_SUITE_HTML = $(TEST_SUITE_LOG:.log=.html)

%.html: %.log
	@for r2h in $(RST2HTML) $$RST2HTML rst2html rst2html.py;	\
	do								\
	  if ($$r2h --version) >/dev/null 2>&1; then			\
	    R2H=$$r2h;							\
	  fi;								\
	done;								\
	if test -z "$$R2H"; then					\
	  echo >&2 "cannot find rst2html, cannot create $@";		\
	  exit 2;							\
	fi;								\
	$$R2H $< >$@.tmp
	@mv $@.tmp $@

# Be sure to run check-TESTS first, and then to convert the result.
# Beware of concurrent executions.  Run "check", not "check-TESTS",
# since the dependencies (check_PROGRAMS and others) are attached to
# the former, not the latter.  And expect "check" to fail.
check-html:
	@if $(MAKE) $(AM_MAKEFLAGS) check; then :; else	\
	  rv=$$?;					\
	  $(MAKE) $(AM_MAKEFLAGS) $(TEST_SUITE_HTML);	\
	  exit $$rv;					\
	fi

.PHONY: check-html


## --------- ##
## recheck.  ##
## --------- ##

# Get the list of tests that failed.
LIST_FAILED_TEST_LOGS =							\
  perl -e 'print join " ",						\
                 grep {open(_) && <_> =~ /^(FAIL|XPASS): /} (@ARGV);'	\
       -- $(TEST_LOGS)
.PHONY: recheck recheck-html
recheck recheck-html:
	@target=$$(echo $@ | sed -e 's/^re//');			\
	if test -f $(TEST_SUITE_LOG); then			\
	  TESTS=$$($(LIST_FAILED_TEST_LOGS));			\
	  $(MAKE) $(AM_MAKEFLAGS) $$target TESTS="$$TESTS";	\
	else							\
	  $(MAKE) $(AM_MAKEFLAGS) $$target;			\
	fi

## ------- ##
## Clean.  ##
## ------- ##

check-clean: check-clean-local
	rm -f $(CHECK_CLEANFILES) $(TEST_SUITE_LOG) $(TEST_SUITE_HTML) $(TEST_LOGS)
.PHONY: check-clean check-clean-local
clean-local: check-clean
