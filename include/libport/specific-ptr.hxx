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
#include <boost/bind.hpp>
namespace libport
{
  /// Register the hook for the process.
  template <typename T, typename Key>
  SpecificPtr<T, Key>::SpecificPtr()
    : map_(), key_(), cleaner_(0)
  {
    key_.register_free_hook(boost::bind(&SpecificPtr<T, Key>::remove,
                                        this, _1));
  }

  /// Register the hook for the process and save the \a cleaner.
  template <typename T, typename Key>
  SpecificPtr<T, Key>::SpecificPtr(cleanFunction cleaner)
    : map_(), key_(), cleaner_(cleaner)
  {
    key_.register_free_hook(boost::bind(&SpecificPtr<T, Key>::remove,
                                        this, _1));
  }

  /// iterate on all elements mapped to clean them with the cleaner
  /// function.
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

  /// Fetch the current process ID and lookup if the value is mapped.
  /// Otherwise it returns the default point value which is 0.
  template <typename T, typename Key>
  T*
  SpecificPtr<T, Key>::get()
  {
    typename Key::type current = Key::current();
    return map_[current];
  }

  /// Similar to \fn get.
  template <typename T, typename Key>
  T*
  SpecificPtr<T, Key>::operator->()
  {
    return get();
  }

  /// Similar to \fn get except that the mapped value must be initialized.
  template <typename T, typename Key>
  T&
  SpecificPtr<T, Key>::operator*()
  {
    T* e = get();
    aver(e);
    return *e;
  }

  /// Get the \a current value before mapping the \a new_value.  Clean the
  /// previous value if it is not 0 and if the \a clean argument is set.
  /// This function return the previous value, and it returns 0 if it has
  /// been cleaned.
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

  /// Shortcut on top of \fn reset.
  template <typename T, typename Key>
  void
  SpecificPtr<T, Key>::remove(typename Key::type current)
  {
    reset(0, true, current);
  }

}

#endif
