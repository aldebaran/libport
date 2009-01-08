lib_LTLIBRARIES += libsched.la
dist_libsched_la_SOURCES = $(libsched_sources)
libsched_la_LDFLAGS = -avoid-version -no-undefined
AM_CPPFLAGS += $(LIBSCHED_CPPFLAGS) -DBUILDING_LIBSCHED $(BOOST_CPPFLAGS)

dist_libsched_la_SOURCES +=			\
  $(coroutines_sources)				\
  lib/sched/configuration.cc			\
  lib/sched/exception.hh			\
  lib/sched/exception.hxx			\
  lib/sched/fwd.hh				\
  lib/sched/job.cc				\
  lib/sched/job.hh				\
  lib/sched/job.hxx				\
  lib/sched/pthread-coro.cc			\
  lib/sched/pthread-coro.hh			\
  lib/sched/pthread-coro.hxx			\
  lib/sched/scheduler.cc			\
  lib/sched/scheduler.hh			\
  lib/sched/scheduler.hxx			\
  lib/sched/tag.cc				\
  lib/sched/tag.hh				\
  lib/sched/tag.hxx

coroutines_sources =				\
  lib/sched/coroutine.hh			\
  lib/sched/coroutine.hxx

include lib/sched/libcoroutine/local.mk
