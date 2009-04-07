if ! SCHED_CORO_OSTHREAD
## This test is directly checking the coroutine interface, not
## the sched interface.
TESTS_BINARIES +=				\
		tests/sched/sched.cc	        \
		tests/sched/sched-except.cc
endif

tests_sched_sched_SOURCES = tests/sched/sched.cc
tests_sched_sched_LDFLAGS = $(SCHED_LIBS) $(AM_LDFLAGS)

tests_sched_sched_except_SOURCES = tests/sched/sched-except.cc
tests_sched_sched_except_LDFLAGS = $(SCHED_LIBS) $(AM_LDFLAGS)
