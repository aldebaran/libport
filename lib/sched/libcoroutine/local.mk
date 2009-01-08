coroutines_sources +=				\
  lib/sched/libcoroutine/Base.h			\
  lib/sched/libcoroutine/Common.h		\
  lib/sched/libcoroutine/Coro.cc		\
  lib/sched/libcoroutine/Coro.h			\
  lib/sched/libcoroutine/PortableUContext.c	\
  lib/sched/libcoroutine/PortableUContext.h	\
  lib/sched/libcoroutine/PortableUContext386.h	\
  lib/sched/libcoroutine/PortableUContext_asm.S

# This is distributed, but not compiled directly: it is included by
# Coro.cc.
EXTRA_DIST += lib/sched/libcoroutine/Coro.c
