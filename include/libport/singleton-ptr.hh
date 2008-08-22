#ifndef LIBPORT_SINGLETON_PTR_HH
# define LIBPORT_SINGLETON_PTR_HH

# include <libport/export.hh>

# define STATIC_INSTANCE_(Cl, Name)					\
  class Cl ## Name;							\
  libport::SingletonPtr<Cl ## Name> Name;				\
  template<> Cl ## Name* libport::SingletonPtr<Cl ## Name>::ptr = 0

# define STATIC_INSTANCE_DECL_(Cl, Name)				\
  class Cl ## Name							\
    : public Cl								\
  {};									\
  STATIC_INSTANCE_(Cl, Name)

# define EXTERN_STATIC_INSTANCE(Cl, Name)				\
  class Cl ## Name							\
    : public Cl								\
  {};									\
  extern libport::SingletonPtr<Cl ## Name> Name;

// These _NS version are made to bypass vcxx error C2888
// cf: http://msdn.microsoft.com/en-us/library/27zksbks(VS.80).aspx
// Symbol in libport cannot be defined inside urbi. This appeared
// after changes done for liburbi Java.
// Use them "outside" of any namespace.

# define STATIC_INSTANCE_NS(Cl, Name, NS)				\
  namespace NS {							\
    class Cl ## Name;							\
    libport::SingletonPtr<Cl ## Name> Name;				\
  }									\
  template<> NS::Cl ## Name*						\
  libport::SingletonPtr<NS::Cl ## Name>::ptr = 0

# define STATIC_INSTANCE_DECL_NS(Cl, Name, NS)				\
  namespace NS {							\
    class Cl ## Name							\
      : public Cl							\
    {};									\
  }									\
  STATIC_INSTANCE_NS(Cl, Name, NS)

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

    static LIBPORT_API T* ptr;
  };

}  // namespace libport

#endif // !LIBPORT_SINGLETON_PTR_HH
