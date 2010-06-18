/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**							-*- C++ -*-
 ** \file libport/map.hxx
 ** \brief Implementation of libport::map.
 */

#ifndef LIBPORT_MAP_HXX
# define LIBPORT_MAP_HXX

# include <iostream>
# include <sstream>

# include <libport/deref.hh>
# include <libport/foreach.hh>
# include <libport/format.hh>
# include <libport/indent.hh>
# include <libport/map.hh>

namespace libport
{

  template <class T, class N>
  map<T, N>::map ()
  {
  }


  template <class T, class N>
  map<T, N>*
  map<T, N>::clone () const
  {
    return new map<T, N> (*this);
  }

  template <class T, class N>
  map<T, N>::~map ()
  {
  }

  template <class T, class N>
  typename map<T, N>::const_iterator
  map<T, N>::find (const T& t) const
  {
    return map_.find (t);
  }

  template <class T, class N>
  typename map<T, N>::iterator
  map<T, N>::find (const T& t)
  {
    return map_.find (t);
  }

  template <class T, class N>
  typename map<T, N>::iterator
  map<T, N>::xfind (const T& t) throw (std::range_error)
  {
    iterator ires = find (t);
    if (ires != map_.end ())
      return ires;
    else
      throw std::range_error(libport::format("map: no mapping for %s", t));
  }

  template <class T, class N>
  typename map<T, N>::const_iterator
  map<T, N>::xfind (const T& t) const throw (std::range_error)
  {
    const_iterator ires = find (t);
    if (ires != map_.end ())
      return ires;
    else
      throw std::range_error (libport::format("map: no mapping for %s", t));
  }

  template <class T, class N>
  typename map<T, N>::key_compare
  map<T, N>::key_comp () const
  {
    return map_.key_comp ();
  }

  template <class T, class N>
  N
  map<T, N>::operator() (const T& t) const throw (std::range_error)
  {
    return xfind (t)->second;
  }

  template <class T, class N>
  std::list<N>
  map<T, N>::operator() (const std::list<T>& ts) const throw (std::range_error)
  {
    std::list<N> res;
    foreach (const T& t, ts)
      res.push_back (operator() (t));
    return res;
  }

  template <class T, class N>
  std::ostream&
  map<T, N>::dump (std::ostream& ostr) const
  {
    foreach (const value_type& p, map_)
      ostr << p.first << " -> "
	   << libport::deref << p.second << libport::iendl;
    return ostr;
  }

  template <class T, class N>
  N
  map<T, N>::operator[] (const T& t) const
  {
    return operator() (t);
  }

  template <class T, class N>
  N&
  map<T, N>::operator[] (const T& t)
  {
    return map_[t];
  }

  template <class T, class N>
  typename map<T, N>::iterator
  map<T, N>::begin ()
  {
    return map_.begin ();
  }

  template <class T, class N>
  typename map<T, N>::iterator
  map<T, N>::end ()
  {
    return map_.end ();
  }

  template <class T, class N>
  typename map<T, N>::const_iterator
  map<T, N>::begin () const
  {
    return map_.begin ();
  }

  template <class T, class N>
  typename map<T, N>::const_iterator
  map<T, N>::end () const
  {
    return map_.end ();
  }


  template <class T, class N>
  typename map<T, N>::iterator
  map<T, N>::lower_bound (const T& k)
  {
    return map_.lower_bound (k);
  }

  template <class T, class N>
  typename map<T, N>::const_iterator
  map<T, N>::lower_bound (const T& k) const
  {
    return map_.lower_bound (k);
  }


  template <class T, class N>
  std::pair<typename map<T, N>::iterator, bool>
  map<T, N>::insert (const std::pair<const T, N>& x)
  {
    return map_.insert (x);
  }

  template <class T, class N>
  void
  map<T, N>::insert (const map& other)
  {
    map_.insert (other.begin (), other.end ());
  }

  template <class T, class N>
  bool
  map<T, N>::empty () const
  {
    return map_.empty ();
  }

  template <class T, class N>
  size_t
  map<T, N>::size () const
  {
    return map_.size ();
  }

  template <class T, class N>
  N
  map<T, N>::take (const T& t) throw (std::range_error)
  {
    typename map_type::iterator ires = xfind (t);
    N elt = ires->second;
    map_.erase (ires);
    return elt;
  }

  template <class T, class N>
  void
  map<T, N>::clear ()
  {
    map_.clear ();
  }

  template <class T, class N>
  std::ostream&
  operator<< (std::ostream& ostr, const map<T, N>& m)
  {
    return m.dump (ostr);
  }

}

#endif // !LIBPORT_MAP_HXX
