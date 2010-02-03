/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SPECIFIC_PTR_HXX
# define LIBPORT_SPECIFIC_PTR_HXX

namespace libport
{

  template <typename T, typename Key>
  SpecificPtr<T, Key>::SpecificPtr()
    : map_(), key_(), cleaner_(0)
  {
    key_.register_free_hook(boost::bind(&SpecificPtr<T, Key>::remove,
                                        this, _1));
  }

  template <typename T, typename Key>
  SpecificPtr<T, Key>::SpecificPtr(cleanFunction cleaner)
    : map_(), key_(), cleaner_(cleaner)
  {
    key_.register_free_hook(boost::bind(&SpecificPtr<T, Key>::remove,
                                        this, _1));
  }

  template <typename T, typename Key>
  SpecificPtr<T, Key>::~SpecificPtr()
  {
    typedef std::pair<typename Key::type, T*> elem;

    if (cleaner_)
    {
      foreach(elem e, map_)
        cleaner_(e.second);
    }
    else
    {
      foreach(elem e, map_)
        delete e.second;
    }
  }

  template <typename T, typename Key>
  T*
  SpecificPtr<T, Key>::get()
  {
    typename Key::type current = Key::current();
    return map_[current];
  }

  template <typename T, typename Key>
  T*
  SpecificPtr<T, Key>::operator->()
  {
    return get();
  }

  template <typename T, typename Key>
  T&
  SpecificPtr<T, Key>::operator*()
  {
    return *get();
  }

  template <typename T, typename Key>
  T*
  SpecificPtr<T, Key>::reset(T* new_value, bool clean,
                             typename Key::type current)
  {
    T* current_value = map_[current];
    if(current_value != new_value)
    {
      map_[current] = new_value;
      key_.set_hook();
      if (clean && current_value)
      {
        if (cleaner_)
          cleaner_(current_value);
        else
          delete current_value;
        return 0;
      }
    }
    return current_value;
  }

  template <typename T, typename Key>
  void
  SpecificPtr<T, Key>::remove(typename Key::type current)
  {
    reset(0, true, current);
  }

}

#endif
