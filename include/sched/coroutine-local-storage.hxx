/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_THREAD_LOCAL_STORAGE_HXX
# define SCHED_THREAD_LOCAL_STORAGE_HXX

# include <sched/coroutine.hh>

namespace sched
{
  template <typename T>
  T&
  CoroutineLocalStorage<T>::get()
  {
    Coro* token = coroutine_current();
    typename map_type::iterator it = map_.find(token);
    if (it == map_.end())
    {
      T* res = new T;
      add_coroutine_free_hook
        (boost::bind(&CoroutineLocalStorage<T>::cleanup_, this, _1));
      map_[token] = res;
      return *res;
    }
    else
      return *it->second;
  }

  template <typename T>
  void
  CoroutineLocalStorage<T>::cleanup_(Coro* coro)
  {
    typename map_type::iterator it = map_.find(coro);
    if (it != map_.end())
    {
      delete it->second;
      map_.erase(it);
    }
  }

  template <typename T>
  const T&
  CoroutineLocalStorage<T>::get() const
  {
    return const_cast<CoroutineLocalStorage<T>*>(this)->get();
  }

  template <typename T>
  T&
  CoroutineLocalStorage<T>::operator * ()
  {
    return get();
  }

  template <typename T>
  const T&
  CoroutineLocalStorage<T>::operator * () const
  {
    return get();
  }

  template <typename T>
  T*
  CoroutineLocalStorage<T>::operator -> ()
  {
    return get();
  }

  template <typename T>
  const T*
  CoroutineLocalStorage<T>::operator -> () const
  {
    return get();
  }
}

#endif
