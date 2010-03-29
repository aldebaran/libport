/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/pod-cast.hh
/// \brief Wrap C++ objects into PODs.

#ifndef LIBPORT_POD_CAST_HH
# define LIBPORT_POD_CAST_HH

# include <iosfwd>
# include <boost/static_assert.hpp>

namespace libport
{
  /// Cast an entity of type T into one of type U.
  ///
  /// This can be used to store C++ non POD types into PODs,
  /// for instance in order to embed them into a union.
  ///
  /// Of course, no ctor/dtor: the whole point is to get a POD.
  template <typename T, typename U = long long>
  class pod_caster
  {
  public:
    /// Store \a s into this.
    pod_caster<T, U>& operator=(const T& s);

    /// Get back the object with its original type.
    T value() const;

    /// Convenience wrapper around the previous function
    operator T () const;

    /// Dump on \a o.
    std::ostream& dump(std::ostream& o) const;

  private:
    BOOST_STATIC_ASSERT(sizeof(T) <= sizeof(U));
    U value_;
  };

  template <typename T, typename U>
  std::ostream& operator<<(std::ostream& o, const pod_caster<T, U>& p);

  /// Convenience wrapper.
  template <typename T, typename U>
  pod_caster<T, U> pod_cast(const T& s);
}

# include <libport/pod-cast.hxx>

#endif // !LIBPORT_POD_CAST_HH
