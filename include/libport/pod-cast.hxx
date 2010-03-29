/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/pod-cast.hxx
/// \brief Wrap C++ objects into PODs.

#ifndef LIBPORT_POD_CAST_HXX
# define LIBPORT_POD_CAST_HXX

#include <libport/pod-cast.hh>
#include <libport/intrusive-ptr.hh>

namespace libport
{

  template <typename T>
  struct SpecialActions
  {
      static inline void onWrite(const T&)
      {}
      static inline void onRead(const T&)
      {}
  };

  template <typename T>
  struct SpecialActions<libport::intrusive_ptr<T> >
  {
      static inline void onWrite(const libport::intrusive_ptr<T>& p)
      {
        if (p)
          p->counter_inc();
      }
      static inline void onRead(const libport::intrusive_ptr<T>& /*p*/)
      {
        // FIXME: This imposes very strict and carefull use of the
        // pod. One and only one read must be performed
//         if (p)
//           p->counter_dec();
      }
  };

  template <typename T, typename U>
  pod_caster<T, U>&
  pod_caster<T, U>::operator=(const T& s)
  {
    value_ = reinterpret_cast<U&>(const_cast<T&>(s));
    SpecialActions<T>::onWrite(reinterpret_cast<T&>(value_));
    return *this;
  }

  template <typename T, typename U>
  T
  pod_caster<T, U>::value () const
  {
    SpecialActions<T>::onRead(const_cast<T&>(reinterpret_cast<const T&>(value_)));
    return const_cast<T&>(reinterpret_cast<const T&>(value_));
  }

  template <typename T, typename U>
  pod_caster<T, U>::operator T () const
  {
    return value();
  }

  template <typename T, typename U>
  std::ostream&
  pod_caster<T, U>::dump(std::ostream& o) const
  {
    return o << value();
  }

  template <typename T, typename U>
  std::ostream&
  operator<<(std::ostream& o, const pod_caster<T, U>& p)
  {
    return p.dump(o);
  }

  template <typename T, typename U>
  pod_caster<T, U>
  pod_cast(const T& s)
  {
    return pod_caster<T, U>() = s;
  }
}

#endif // !LIBPORT_POD_CAST_HXX

