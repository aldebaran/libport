/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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

  inline void
  UniquePointer::clear()
  {
    set_.clear();
  }

}

#endif // !LIBPORT_UNIQUE_POINTER_HXX
