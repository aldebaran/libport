#ifndef SCHED_COROUTINE_CORO_HXX
# define SCHED_COROUTINE_CORO_HXX

inline Coro*
coroutine_new(size_t stack_size)
{
  Coro* res = Coro_new();
  Coro_setStackSize_(res,
                     (stack_size
                      ? stack_size
                      : sched::configuration.default_stack_size));
  return res;
}

inline void
coroutine_free(Coro* coro)
{
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
  Coro_initializeMainCoro(coro);
}

template<typename T>
inline void
coroutine_start(Coro* self, Coro* other, void (*callback)(T*), T* context)
{
  Coro_startCoro_(self, other, context,
		  reinterpret_cast<CoroStartCallback*>(callback));
}

inline void
coroutine_switch_to(Coro* self, Coro* next)
{
  Coro_switchTo_(self, next);
}

#endif // SCHED_COROUTINE_CORO_HXX
