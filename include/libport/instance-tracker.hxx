/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_INSTANCE_TRACKER_HXX
# define LIBPORT_INSTANCE_TRACKER_HXX

namespace libport
{
  template <typename T>
  InstanceTracker<T>::InstanceTracker()
  {
    instances_.insert(static_cast<T*>(this));
  }

  template <typename T>
  InstanceTracker<T>::~InstanceTracker()
  {
    instances_.erase(instances_.find(static_cast<T*>(this)));
  }

  template <typename T>
  const typename InstanceTracker<T>::set_type&
  InstanceTracker<T>::instances_get()
  {
    return instances_;
  }

  template <typename T>
  bool InstanceTracker<T>::check(T* inst) const
  {
    return libport::mhas(instances_, inst);
  }

  template <typename T>
  typename InstanceTracker<T>::set_type InstanceTracker<T>::instances_;
}

#endif
