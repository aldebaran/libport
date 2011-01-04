## Copyright (C) 2009-2010, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

if ! SCHED_CORO_OSTHREAD
## This test is directly checking the coroutine interface, not
## the sched interface.
TESTS_BINARIES +=				\
  tests/sched/sched.cc				\
  tests/sched/sched-except.cc			\
  tests/sched/debug.cc
endif

tests_sched_debug_SOURCES = tests/sched/debug.cc
tests_sched_debug_LDFLAGS = $(SCHED_LIBS) $(AM_LDFLAGS)

tests_sched_sched_SOURCES = tests/sched/sched.cc
tests_sched_sched_LDFLAGS = $(SCHED_LIBS) $(AM_LDFLAGS)

tests_sched_sched_except_SOURCES = tests/sched/sched-except.cc
tests_sched_sched_except_LDFLAGS = $(SCHED_LIBS) $(AM_LDFLAGS)
