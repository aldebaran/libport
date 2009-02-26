#ifndef SCHED_COROUTINE_HXX
# define SCHED_COROUTINE_HXX

// Implementation based on libcoroutine.
# if !defined LIBPORT_SCHED_CORO_OSTHREAD
#  include <sched/coroutine-coro.hxx>
# endif

// Implementation independendant routines.

inline size_t
coroutine_bytes_left_on_stack(Coro* self)
{
  char dummy;
  char* p1 = &dummy;
  char* p2 = (char*) coroutine_current_stack_pointer(self);
  bool stack_moves_up = p2 > p1;
  char* start = (char*) coroutine_stack_addr(self);

  // FIXME: Use Autoconf instead?
  if (stack_moves_up) // like PPC
  {
    char* end = start + coroutine_stack_size(self);
    return end - p1;
  }
  else // like x86
    return p1 - start;
}

inline bool
coroutine_stack_space_almost_gone(Coro* coro)
{
  return (coroutine_bytes_left_on_stack(coro)
          < sched::configuration.minimum_stack_size);
}

#endif // SCHED_COROUTINE_HXX
