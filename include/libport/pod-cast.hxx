/// \file libport/pod-cast.hxx
/// \brief Wrap C++ objects into PODs.

#ifndef LIBPORT_POD_CAST_HXX
# define LIBPORT_POD_CAST_HXX

#include <libport/pod-cast.hh>

namespace libport
{

  template <typename T, typename U>
  pod_caster<T, U>&
  pod_caster<T, U>::operator=(const T& s)
  {
    value_ = reinterpret_cast<U&>(const_cast<T&>(s));
    return *this;
  }
  
  template <typename T, typename U>
  T
  pod_caster<T, U>::value () const
  {
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

