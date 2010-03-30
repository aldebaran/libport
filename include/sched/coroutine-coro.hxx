/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_COROUTINE_CORO_HXX
# define SCHED_COROUTINE_CORO_HXX

# include <libport/local-data.hh>
# include <libport/thread-data.hh>

# ifndef __UCLIBC__
typedef ::libport::LocalSingleton<Coro*, ::libport::localdata::Thread>
  LocalCoroPtr;
# else
 /* Under uclibc, we need pthread local data to be coroutine-specific for proper
  * exception handling. So current_coro ptr cannot be a pthread local.
  */
typedef Coro* LocalCoroPtr;
# endif

// Hook stuff for uclibc-workaround
SCHED_API extern LocalCoroPtr coroutine_current_;
SCHED_API extern Coro* coroutine_main_;
SCHED_API extern void (*coroutine_free_hook)(Coro*);
SCHED_API extern void (*coroutine_new_hook) (Coro*);
// Hack to force inclusion of hook object when the user links against a static
// libsched
SCHED_API void coroutine_force_hook_linkage();

inline Coro*
coroutine_current()
{
  return coroutine_current_;
}

inline Coro*
coroutine_main()
{
  return coroutine_main_;
}

inline Coro*
coroutine_new(size_t stack_size)
{
  Coro* res = Coro_new();
  Coro_setStackSize_(res,
                     (stack_size
                      ? stack_size
                      : sched::configuration.default_stack_size));
  if (coroutine_new_hook)
    coroutine_new_hook(res);
  return res;
}

inline void
coroutine_free(Coro* coro)
{
  if (coroutine_free_hook)
    coroutine_free_hook(coro);
  Coro_free(coro);
}

inline void*
coroutine_current_stack_pointer(Coro*)
{
  return Coro_CurrentStackPointer();
}

inline void*
coroutine_stack_addr(Coro* self)
{
  return self->stack;
}

inline size_t
coroutine_stack_size(Coro* self)
{
  return self->requestedStackSize;
}

inline void
coroutine_initialize_main(Coro* coro)
{
  coroutine_main_ = coro;
  coroutine_current_ = coro;
  Coro_initializeMainCoro(coro);
  coroutine_force_hook_linkage();
}

template<typename T>
inline void
coroutine_start(Coro* self, Coro* other, void (*callback)(T*), T* context)
{
  coroutine_current_ = other;
  Coro_startCoro_(self, other, context,
		  reinterpret_cast<CoroStartCallback*>(callback));
  coroutine_current_ = self;
}

inline void
coroutine_switch_to(Coro* self, Coro* next)
{
  coroutine_current_ = next;
  Coro_switchTo_(self, next);
  coroutine_current_ = self;
}

#endif // SCHED_COROUTINE_CORO_HXX
