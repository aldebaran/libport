/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_COROUTINE_DATA_HXX
# define SCHED_COROUTINE_DATA_HXX

SCHED_API extern void (*coroutine_free_hook)(Coro*);

namespace sched
{
  template <typename T>
  CoroutineKey<T>::CoroutineKey()
  {
  }

  template <typename T>
  CoroutineKey<T>::~CoroutineKey()
  {
  }

  template <typename T>
  typename CoroutineKey<T>::type
  CoroutineKey<T>::current()
  {
    return coroutine_current();
  }

  template <typename T>
  void
  CoroutineKey<T>::register_free_hook(boost::function1<void, type> free)
  {
    add_coroutine_free_hook(free);
  }

  template <typename T>
  void
  CoroutineKey<T>::set_hook()
  {
  }

}

#endif

