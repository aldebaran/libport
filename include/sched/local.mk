## Copyright (C) 2009-2010, 2012, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

sched_includedir = $(includedir)/sched
sched_include_HEADERS =				\
  include/sched/configuration.hh		\
  include/sched/coroutine.hh			\
  include/sched/coroutine.hxx			\
  include/sched/coroutine-coro.hxx		\
  include/sched/coroutine-data.hh		\
  include/sched/coroutine-data.hxx		\
  include/sched/coroutine-hooks.hh		\
  include/sched/coroutine-local-storage.hh	\
  include/sched/coroutine-local-storage.hxx	\
  include/sched/exception.hh			\
  include/sched/exception.hxx			\
  include/sched/export.hh			\
  include/sched/fwd.hh				\
  include/sched/job.hh				\
  include/sched/job.hxx				\
  include/sched/scheduler.hh			\
  include/sched/scheduler.hxx			\
  include/sched/tag.hh				\
  include/sched/tag.hxx

libcoroutine_includedir = $(sched_includedir)/libcoroutine
libcoroutine_include_HEADERS =			\
  include/sched/libcoroutine/386-ucontext.h	\
  include/sched/libcoroutine/Common.h		\
  include/sched/libcoroutine/Coro.h		\
  include/sched/libcoroutine/amd64-ucontext.h	\
  include/sched/libcoroutine/power-ucontext.h	\
  include/sched/libcoroutine/taskimpl.h

