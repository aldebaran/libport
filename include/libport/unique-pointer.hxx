#ifndef LIBPORT_UNIQUE_POINTER_HXX
# define LIBPORT_UNIQUE_POINTER_HXX

#include <iostream>
# include <libport/containers.hh>

namespace libport
{
  template <typename T>
  inline
  bool
  UniquePointer::operator() (T* t)
  {
    // We don't care about multiple registrations of 0.
    if (!t)
      return true;
    else if (libport::mhas(set_, t))
      return false;
    else
    {
      set_.insert (t);
      return true;
    }
  }

  template <typename T>
  inline
  bool
  UniquePointer::operator() (T& t)
  {
    return operator()(&t);
  }

}

#endif // !LIBPORT_UNIQUE_POINTER_HXX
