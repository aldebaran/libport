#ifndef LIBPORT_SINGLETON_PTR_HH
# define LIBPORT_SINGLETON_PTR_HH

# define STATIC_INSTANCE(Cl, Name)		\
  libport::SingletonPtr<Cl ## Name> Name

# define EXTERN_STATIC_INSTANCE(Cl, Name)	\
  class Cl ## Name				\
    : public Cl					\
  {};						\
  extern libport::SingletonPtr<Cl ## Name> Name


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
