dist_lib_sched_libsched@LIBSFX@_la_SOURCES +=	\
  lib/sched/libcoroutine/Base.h			\
  lib/sched/libcoroutine/Coro.cc		\
  lib/sched/libcoroutine/context.cc		\
  lib/sched/libcoroutine/asm.S

lib_sched_libsched@LIBSFX@_la_CPPFLAGS +=	\
  -DBUILDING_CORO_DLL
