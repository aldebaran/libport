/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_FINALLY_HXX
# define LIBPORT_FINALLY_HXX

#include <boost/lambda/lambda.hpp>

namespace libport
{
  // Implementation note: since an allocation is needed to store multiple
  // allocations, we reserve some room so that up to four actions may be
  // stored before any reallocation takes place.

  inline
  Finally::Finally()
  {
    actions_.reserve(4);
  }

  inline
  Finally::Finally(const Finally& f)
    : actions_(f.actions_)
  {
    f.actions_.clear();
  }

  inline
  Finally::Finally(const action_type& a)
  {
    actions_.reserve(4);
    *this << a;
  }

  inline
  Finally::Finally(unsigned int n)
  {
    actions_.reserve(n);
  }

  inline
  Finally::~Finally()
  {
    foreach (const action_type& a, actions_)
      a();
  }

  inline
  Finally& Finally::operator <<(const action_type& a)
  {
    actions_.push_back(a);
    return *this;
  }

  template <typename T>
  static void set(T* what, const T& value)
  {
    *what = value;
  }

  template<typename T>
  inline
  Finally::action_type restore(T& v)
  {
    return boost::bind(set<T>, &v, v);
  }

  template<typename T>
  Finally::action_type scoped_set(T& v, T n)
  {
    Finally::action_type res = restore(v);
    v = n;
    return res;
  }

  template<typename Value, typename Container>
  Finally::action_type scoped_push(const Value& value, Container& container)
  {
    container.push_back(value);
    return Finally::action_type(boost::bind(&Container::pop_back, &container));
  }
}


#endif
