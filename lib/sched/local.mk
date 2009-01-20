lib_LTLIBRARIES += lib/sched/libsched.la
dist_lib_sched_libsched_la_SOURCES = $(lib_sched_libsched_sources)
lib_sched_libsched_la_LDFLAGS = -avoid-version -no-undefined
lib_sched_libsched_la_LIBADD = $(LIBADD) $(libport_la)
lib_sched_libsched_la_CPPFLAGS =		\
  $(AM_CPPFLAGS)				\
  -I$(srcdir)/lib				\
  -DBUILDING_LIBSCHED				\
  $(BOOST_CPPFLAGS)

dist_lib_sched_libsched_la_SOURCES +=		\
  lib/sched/configuration.cc			\
  lib/sched/job.cc				\
  lib/sched/pthread-coro.cc			\
  lib/sched/pthread-coro.hh			\
  lib/sched/pthread-coro.hxx			\
  lib/sched/scheduler.cc			\
  lib/sched/tag.cc				\
  lib/sched/libcoroutine/Base.h			\
  lib/sched/libcoroutine/Coro.cc		\
  lib/sched/libcoroutine/PortableUContext.c	\
  lib/sched/libcoroutine/PortableUContext_asm.S

# This is distributed, but not compiled directly: it is included by
# Coro.cc.
EXTRA_DIST += lib/sched/libcoroutine/Coro.c
