/**
 ** \file  libport/deref.hxx
 ** \brief Inline methods for libport/deref.hh.
 **/

#ifndef LIBPORT_DEREF_HXX
# define LIBPORT_DEREF_HXX

# include <iostream>
# include "libport/deref.hh"
# include "libport/contract.hh"

namespace libport
{
  template <typename T>
  std::ostream&
  Deref::operator<< (const T* t) const
  {
    if (t)
      return ostr_ << *t;
    else
      return ostr_ << "NULL";
  }

  template <typename T>
  std::ostream&
  Deref::operator<< (T* t) const
  {
    if (t)
      return ostr_ << *t;
    else
      return ostr_ << "NULL";
  }

  template <typename T>
  std::ostream&
  Deref::operator<< (const shared_ptr<T>& t) const
  {
    if (t)
      return ostr_ << *t;
    else
      return ostr_ << "NULL";
  }

  template <typename T>
  std::ostream&
  Deref::operator<< (const T& t) const
  {
    return ostr_ << t;
  }

  template <typename T>
  std::ostream&
  Deref::operator<< (T& t) const
  {
    return ostr_ << t;
  }

  inline
  Deref
  operator<< (std::ostream& ostr, deref_e)
  {
    return Deref (ostr);
  }
}

#endif // !LIBPORT_DEREF_HXX
