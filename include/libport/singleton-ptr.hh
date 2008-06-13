#ifndef LIBPORT_SINGLETON_PTR_HH
# define LIBPORT_SINGLETON_PTR_HH

# define LIBPORT_SINGLETON(Class, Name)         \
  Class ## _ ## Name

# define STATIC_INSTANCE(Class, Name)					\
  class LIBPORT_SINGLETON(Class, Name);                                 \
  libport::SingletonPtr<LIBPORT_SINGLETON(Class, Name)> Name

# define STATIC_INSTANCE_DECL(Class, Name)       \
  class LIBPORT_SINGLETON(Class, Name)           \
    : public Class                               \
  {};                                            \
  STATIC_INSTANCE(Class, Name)

# define EXTERN_STATIC_INSTANCE(Class, Name)				\
  class LIBPORT_SINGLETON(Class, Name)                                  \
    : public Class                                                      \
  {};									\
  extern libport::SingletonPtr<LIBPORT_SINGLETON(Class, Name)> Name

namespace libport
{
  /// Singleton smart pointer that creates the object on demand.
  template<class T>
  class SingletonPtr
  {
  public:
    operator T* ()
    {
      return &instance();
    }
    operator T& ()
    {
      return instance();
    }

    T* operator ->()
    {
      return &instance();
    }

  private:
    static T& instance()
    {
      static T t;
      return t;
    }
  };

}  // namespace libport

#endif // !LIBPORT_SINGLETON_PTR_HH
