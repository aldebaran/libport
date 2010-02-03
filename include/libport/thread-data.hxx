/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_THREAD_DATA_HXX
# define LIBPORT_THREAD_DATA_HXX

namespace libport
{
  namespace _impl
  {
    template <typename T>
    void
    call_thread_key_cleanup(void* tk)
    {
      if (tk)
        static_cast< ThreadKey<T>* >(tk)->cleanup();
    }
  }

  template <typename T>
  ThreadKey<T>::ThreadKey()
  {
    // should check errno
    pthread_key_create(&key, &_impl::call_thread_key_cleanup<T>);
  }

  template <typename T>
  ThreadKey<T>::~ThreadKey()
  {
    // should check errno
    pthread_key_delete(key);
  }

  template <typename T>
  typename ThreadKey<T>::type
  ThreadKey<T>::current()
  {
    return pthread_self();
  }

  template <typename T>
  void
  ThreadKey<T>::register_free_hook(boost::function1<void, type> free)
  {
    free_ = free;
  }

  template <typename T>
  void
  ThreadKey<T>::set_hook()
  {
    pthread_setspecific(key, static_cast<void*>(this));
  }

  template <typename T>
  void
  ThreadKey<T>::cleanup()
  {
    free_(current());
  }
}

#endif
