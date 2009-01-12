TESTS +=					\
  sched/sched.cc

sched_sched_SOURCES = sched/sched.cc
sched_sched_LDFLAGS = $(top_builddir)/lib/sched/libsched.la  $(AM_LDFLAGS)
