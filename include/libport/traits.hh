#ifndef LIBPORT_TRAITS_HH
# define LIBPORT_TRAITS_HH

# include <libport/intrusive-ptr.hh>

namespace libport
{
  namespace traits
  {
    template <typename T>
    struct Id
    {
      typedef T res;
    };

    template <typename T>
    struct Const
    {
      typedef const T res;
    };

    template <typename T>
    struct Ptr
    {
      typedef T* res;
    };

    template <typename T>
    struct Ref
    {
      typedef T& res;
    };

    template <typename T>
    struct ConstPtr
    {
      typedef typename meta::Compose<Ptr, Const>::Res<T>::res res;
    };

    template <typename T>
    struct ConstRef
    {
      typedef typename meta::Compose<Ref, Const>::Res<T>::res res;
    };

    template <typename T>
    struct SharedPtr
    {
      typedef typename libport::intrusive_ptr<T> res;
    };

    template <typename T>
    struct ConstSharedPtr
    {
      typedef typename meta::Compose<SharedPtr, Const>::Res<T>::res res;
    };
  }
}

#endif
