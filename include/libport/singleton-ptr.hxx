/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SINGLETON_PTR_HXX
# define LIBPORT_SINGLETON_PTR_HXX

# include <libport/singleton-ptr.hh>


namespace libport
{
  template<typename T>
  SingletonPtr<T>::operator T* ()
  {
    return instance();
  }

  template<typename T>
  SingletonPtr<T>::operator T& ()
  {
    return *instance();
  }

  template<typename T>
  T*
  SingletonPtr<T>::operator ->()
  {
    return instance();
  }

  template<typename T>
  T*
  SingletonPtr<T>::instance()
  {
    if (!ptr)
      ptr = new T();
    return ptr;
  }

}  // namespace libport

#endif // !LIBPORT_SINGLETON_PTR_HXX
