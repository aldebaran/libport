## Copyright (C) 2009-2010, Gostai S.A.S.
##
## This software is provided "as is" without warranty of any kind,
## either expressed or implied, including but not limited to the
## implied warranties of fitness for a particular purpose.
##
## See the LICENSE file for more information.

lib_LTLIBRARIES += lib/sched/libsched@LIBSFX@.la
lib_sched_libsched@LIBSFX@_la_LDFLAGS = -avoid-version -no-undefined
lib_sched_libsched@LIBSFX@_la_LIBADD = $(LIBADD) $(LIBPORT_LIBS)
lib_sched_libsched@LIBSFX@_la_CPPFLAGS =	\
  $(AM_CPPFLAGS)				\
  -DBUILDING_LIBSCHED

if PTHREAD_SOURCES
lib_sched_libsched@LIBSFX@_la_CPPFLAGS +=		\
  -I$(PTHREAD_SOURCES)					\
  -I$(PTHREAD_SOURCES)/linuxthreads			\
  -I$(PTHREAD_SOURCES)/linuxthreads/sysdeps/pthread	\
  -I$(PTHREAD_SOURCES)/linuxthreads/sysdeps/arm
endif

dist_lib_sched_libsched@LIBSFX@_la_SOURCES =	\
  lib/sched/configuration.cc			\
  lib/sched/coroutine-hooks.cc			\
  lib/sched/job.cc				\
  lib/sched/pthread-coro.cc			\
  lib/sched/pthread-coro.hh			\
  lib/sched/pthread-coro.hxx			\
  lib/sched/scheduler.cc			\
  lib/sched/tag.cc				\
  lib/sched/uclibc-workaround.cc

lib_sched_libsched@LIBSFX@_la_CPPFLAGS +=	\
  -I$(top_srcdir)/include/sched/libcoroutine

include lib/sched/libcoroutine/local.mk
