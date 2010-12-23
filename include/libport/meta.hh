/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_META_HH
# define LIBPORT_META_HH

# include <libport/errors.hh>

#define TYPE(T)                                 \
  public:                                       \
  typedef T _libport_type_                      \

namespace libport
{
  namespace meta
  {
    /// Uniquify T by combining it with a unique id I
    template <typename T, int I>
    struct Uniquify
    {};

    struct True{};
    struct False{};
    /// Equality tester
    template <typename T1, typename T2>
    struct Eq
    {
      static const bool res = false;
    };

    template <typename T>
    struct Eq<T, T>
    {
      static const bool res = true;
    };

    /// If
    template <bool cond, typename Then=True, typename Else=False>
    struct If
    {
      typedef Then res;
    };

    template <typename Then, typename Else>
    struct If<false, Then, Else>
    {
      typedef Else res;
    };

    /// Inherits
    template <typename C, typename M>
    struct Inherits
    {
      typedef char Yes;
      class No {char dummy[2];};
      static No test(...);
      static Yes test(const M*);
      static C* make();

      static const bool res =
        sizeof (test(make())) == sizeof(Yes) && !Eq<M, void>::res;
    };

    /// Inherited
    template <typename M, typename C>
    struct Inherited
    {
        static const bool res = Inherits<C, M>::res;
    };

    /// Deref
    template <typename T>
    struct Deref
    {
        typedef T res;
        static T& deref(T& e)
        {
          return e;
        }
    };

    template <typename T>
    struct Deref<T*>
    {
        typedef typename Deref<T>::res res;
        static res& deref(T* e)
        {
          return Deref<T>::deref(*e);
        }
    };

    template <typename T>
    typename Deref<T>::res& deref(T& e)
    {
      return Deref<T>::deref(e);
    }

    /// Compose
    template <template <typename> class T1, template <typename> class T2>
    struct Compose
    {
        template <typename T>
        struct Res
        {
            typedef typename T1<typename T2<T>::res>::res res;
        };
    };

  }
}


#endif
