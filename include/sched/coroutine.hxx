/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_COROUTINE_HXX
# define SCHED_COROUTINE_HXX

# include <sched/configuration.hh>

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
    return size_t(end - p1);
  }
  else // like x86
    return size_t(p1 - start);
}

inline bool
coroutine_stack_space_almost_gone(Coro* coro)
{
  return (coroutine_bytes_left_on_stack(coro)
          < sched::configuration.minimum_stack_size);
}

#endif // SCHED_COROUTINE_HXX
