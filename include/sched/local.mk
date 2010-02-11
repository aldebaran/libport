sched_includedir = $(libport_include_basedir)/sched
sched_include_HEADERS =					\
  include/sched/configuration.hh			\
  include/sched/coroutine.hh				\
  include/sched/coroutine.hxx				\
  include/sched/coroutine-coro.hxx			\
  include/sched/coroutine-data.hh			\
  include/sched/coroutine-data.hxx			\
  include/sched/coroutine-hooks.hh			\
  include/sched/exception.hh				\
  include/sched/exception.hxx				\
  include/sched/export.hh				\
  include/sched/fwd.hh					\
  include/sched/job.hh					\
  include/sched/job.hxx					\
  include/sched/scheduler.hh				\
  include/sched/scheduler.hxx				\
  include/sched/tag.hh					\
  include/sched/tag.hxx

include include/sched/libcoroutine/local.mk
