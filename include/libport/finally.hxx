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
  Finally::Finally(const Action& a)
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
    foreach (const Action& a, actions_)
      a();
  }

  inline
  Finally& Finally::operator <<(const Action& a)
  {
    actions_.push_back(a);
    return *this;
  }

  template<typename T>
  inline
  Finally::Action restore(T& v)
  {
    return boost::lambda::var(v) = v;
  }

  template<typename T>
  Finally::Action scoped_set(T& v, T n)
  {
    Finally::Action res = restore(v);
    v = n;
    return res;
  }
}


#endif
