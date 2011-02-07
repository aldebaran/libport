/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SINGLETON_PTR_HH
# define LIBPORT_SINGLETON_PTR_HH

# if !defined BUILDING_LIBPORT
#  warning "Libport.SingletonPtr is deprecated."
#  warning "If you use it, please report to libport maintainers."
# endif

# include <libport/export.hh>

# define STATIC_INSTANCE_(Cl, Name)                             \
  class Cl ## Name;                                             \
  libport::SingletonPtr<Cl ## Name> Name;                       \
  namespace libport                                             \
  {                                                             \
    template<> Cl ## Name* SingletonPtr<Cl ## Name>::ptr = 0;   \
  }


# define STATIC_INSTANCE_DECL_(Cl, Name)        \
  class Cl ## Name                              \
    : public Cl                                 \
  {};                                           \
  STATIC_INSTANCE_(Cl, Name)


# define EXTERN_STATIC_INSTANCE_EX(Cl, Name, Api)       \
  class Api Cl ## Name                                  \
    : public Cl                                         \
  {};                                                   \
  Api extern libport::SingletonPtr<Cl ## Name> Name;


# define EXTERN_STATIC_INSTANCE(Cl, Name)               \
  EXTERN_STATIC_INSTANCE_EX(Cl, Name, /* No API. */)

// These _NS version are made to bypass vcxx error C2888
// cf: http://msdn.microsoft.com/en-us/library/27zksbks(VS.80).aspx
// Symbol in libport cannot be defined inside urbi. This appeared
// after changes done for liburbi Java.
// Use them "outside" of any namespace.

# define STATIC_INSTANCE_NS_EX(Cl, Name, NS, Api)       \
  namespace NS                                          \
  {							\
    Api libport::SingletonPtr<Cl ## Name> Name;         \
  }                                                     \
  namespace libport                                     \
  {                                                     \
    template<> NS::Cl ## Name*                          \
      SingletonPtr<NS::Cl ## Name>::ptr = 0;            \
  }

# define STATIC_INSTANCE_NS(Cl, Name, NS)               \
  STATIC_INSTANCE_NS_EX(Cl, Name, NS, /* No API */)

# define STATIC_INSTANCE_DECL_NS(Cl, Name, NS)  \
  namespace NS                                  \
  {                                             \
    class Cl ## Name                            \
      : public Cl                               \
    {};                                         \
  }                                             \
  STATIC_INSTANCE_NS(Cl, Name, NS)

namespace libport
{
  /// Singleton smart pointer that creates the object on demand.
  template<typename T>
  class SingletonPtr
  {
  public:
    operator T* ();
    operator T& ();
    T* operator ->();

  private:
    static T* instance();
    static T* ptr;
  };

}  // namespace libport

# include <libport/singleton-ptr.hxx>

#endif // !LIBPORT_SINGLETON_PTR_HH
