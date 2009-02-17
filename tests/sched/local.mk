TESTS_BINARIES +=				\
  tests/sched/sched.cc

tests_sched_sched_SOURCES = tests/sched/sched.cc
tests_sched_sched_LDFLAGS = $(SCHED_LIBS) $(AM_LDFLAGS)
