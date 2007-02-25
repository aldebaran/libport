#ifndef LIBPORT_SINGLETON_PTR_HH
# define LIBPORT_SINGLETON_PTR_HH

/// Define a singleton of class \a Class named \a Name.
# define STATIC_INSTANCE(Class, Name)		\
  libport::SingletonPtr<Class> Name

/// Declare a singleton of class \a Class named \a Name.
# define EXTERN_STATIC_INSTANCE(Class, Name)	\
  extern STATIC_INSTANCE(Class, Name)


namespace libport
{
  /// Singleton smart pointer that creates the object on demand.
  template<class T>
  class SingletonPtr
  {
  public:
    operator T* ()
    {
      return instance();
    }
    operator T& ()
    {
      return *instance();
    }

    T* operator ->()
    {
      return instance();
    }

  private:
    T* instance()
    {
      static T* ptr = 0;
      if (!ptr)
	ptr = new T();
      return ptr;
    }
  };

}  // namespace libport

#endif // !LIBPORT_SINGLETON_PTR_HH
