/**
 ** \file  libport/deref.hxx
 ** \brief Inline methods for libport/deref.hh.
 **/

#ifndef LIBPORT_DEREF_HXX
# define LIBPORT_DEREF_HXX

# include "libport/deref.hh"
# include "libport/contract.hh"

namespace libport
{
  template <typename T>
  std::ostream&
  Deref::operator<< (const T* t) const
  {
    return this->ostr_ << *t;
  }

  template <typename T>
  std::ostream&
  Deref::operator<< (T* t) const
  {
    return this->ostr_ << *t;
  }

  template <typename T>
  std::ostream&
  Deref::operator<< (const shared_ptr<T>& t) const
  {
    return this->ostr_ << *t;
  }

  template <typename T>
  std::ostream&
  Deref::operator<< (const T& t) const
  {
    return this->ostr_ << t;
  }

  template <typename T>
  std::ostream&
  Deref::operator<< (T& t) const
  {
    return this->ostr_ << t;
  }

  inline
  Deref
  operator<< (std::ostream& ostr, deref_e)
  {
    return Deref (ostr);
  }
}

#endif // !LIBPORT_DEREF_HXX
