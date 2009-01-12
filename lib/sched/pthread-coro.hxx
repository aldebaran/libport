#ifndef SCHED_PTHREAD_CORO_HXX
# define SCHED_PTHREAD_CORO_HXX

// Implemented in the *.cc.
void
coroutine_start(Coro* self, Coro* other,
                void (*callback)(void*), void* context);

template <typename T>
inline
void
coroutine_start(Coro* self, Coro* other, void (*callback)(T*), T* context)
{
  coroutine_start(self, other,
                  reinterpret_cast<void(*)(void*)>(callback), context);
}


#endif // !SCHED_PTHREAD_CORO_HH
