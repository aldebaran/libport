#ifndef LIBPORT_SINGLETON_PTR_HH
# define LIBPORT_SINGLETON_PTR_HH

# define STATIC_INSTANCE(Cl, Name)					\
  class Cl ## Name;							\
  libport::SingletonPtr<Cl ## Name> Name;				\
  template<> Cl ## Name* libport::SingletonPtr<Cl ## Name>::ptr = 0

# define STATIC_INSTANCE_DECL(Cl, Name)		\
  class Cl ## Name				\
    : public Cl					\
  {};						\
  STATIC_INSTANCE(Cl, Name)

# define EXTERN_STATIC_INSTANCE(Cl, Name)				\
  class Cl ## Name							\
    : public Cl								\
  {};									\
  extern libport::SingletonPtr<Cl ## Name> Name;			\
  template <> extern Cl ## Name* libport::SingletonPtr<Cl ## Name>::ptr


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
    static T* instance()
    {
      if (!ptr)
	ptr = new T();
      return ptr;
    }

    static T* ptr;
  };

}  // namespace libport

#endif // !LIBPORT_SINGLETON_PTR_HH
