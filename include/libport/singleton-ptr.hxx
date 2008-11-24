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
