TESTS +=					\
  sched/sched.cc

sched_sched_SOURCES = sched/sched.cc
sched_sched_LDFLAGS = $(SCHED_LIBS) $(AM_LDFLAGS)
