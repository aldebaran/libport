/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/separate.hxx
 ** \brief Output containers with a separator between items.
 */

#ifndef LIBPORT_SEPARATE_HXX
# define LIBPORT_SEPARATE_HXX

# include <ostream>

# include <boost/range.hpp>

# include <libport/deref.hh>
# include <libport/foreach.hh>
# include <libport/separate.hh>

namespace libport
{

  template <typename R, typename S>
  inline
  separator<R, S>::separator (const R& r, const S& s)
    : container_ (r)
    , separator_ (s)
  {
  }

  template <typename R, typename S>
  inline std::ostream&
  separator<R, S>::operator() (std::ostream& o) const
  {
    bool first = true;
    typedef typename boost::range_value<R>::type value_type;
    foreach (const value_type& e, container_)
    {
      if (!first)
        o << separator_;
      o << deref << e;
      first = false;
    }
    return o;
  }

  template <typename R, typename S>
  separator<R, S>
  separate(const R& r, const S& s)
  {
    return separator<R, S> (r, s);
  }

  template <typename R, typename S>
  inline std::ostream&
  operator<< (std::ostream& o, const separator<R, S>& s)
  {
    return s(o);
  }

}

#endif // !LIBPORT_SEPARATE_HXX
