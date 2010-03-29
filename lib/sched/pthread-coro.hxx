/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
