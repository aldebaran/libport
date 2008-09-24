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
  typename InstanceTracker<T>::set_type InstanceTracker<T>::instances_;
}

#endif
