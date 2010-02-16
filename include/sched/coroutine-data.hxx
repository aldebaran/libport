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
  inline
  CoroutineKey<T>::CoroutineKey()
  {
  }

  template <typename T>
  inline
  CoroutineKey<T>::~CoroutineKey()
  {
  }

  /// Return the unmutable identity of coroutines and handle the case where
  /// the main coroutine is changing its identity.
  template <typename T>
  inline
  typename CoroutineKey<T>::type
  CoroutineKey<T>::current()
  {
    type t = coroutine_current();
    // Handle when the main coroutine is changing its identity from
    // Anonymous (null pointer) to a known coroutine.
    if (t == coroutine_main())
      return type();
    return t;
  }

  template <typename T>
  inline
  void
  CoroutineKey<T>::register_free_hook(boost::function1<void, type> free)
  {
    add_coroutine_free_hook(free);
  }

  template <typename T>
  inline
  void
  CoroutineKey<T>::set_hook()
  {
  }

}

#endif

