/**
 ** \file  libport/deref.hxx
 ** \brief Inline methods for libport/deref.hh.
 **/

#ifndef LIBPORT_DEREF_HXX
# define LIBPORT_DEREF_HXX

# include <iostream>

# include <libport/deref.hh>
# include <libport/contract.hh>

namespace libport
{

  /*--------.
  | Deref.  |
  `--------*/

  template <typename T>
  inline
  std::ostream&
  Deref::operator<< (const T* t) const
  {
    if (t)
      return ostr_ << *t;
    else
      return ostr_ << "NULL";
  }

  template <typename T>
  inline
  std::ostream&
  Deref::operator<< (T* t) const
  {
    return operator<<(const_cast<const T*>(t));
  }

  template <typename T>
  inline
  std::ostream&
  Deref::operator<< (const std::auto_ptr<T>& t) const
  {
    return operator <<(t.get());
  }

  template <typename T>
  inline
  std::ostream&
  Deref::operator<< (const intrusive_ptr<T>& t) const
  {
    return operator <<(t.get());
  }

# ifndef LIBPORT_NO_BOOST
  template <typename T>
  inline
  std::ostream&
  Deref::operator<< (const boost::shared_ptr<T>& t) const
  {
    return operator <<(t.get());
  }
# endif

  template <typename T>
  inline
  std::ostream&
  Deref::operator<< (const T& t) const
  {
    return ostr_ << t;
  }


  template <typename T>
  inline
  std::ostream&
  Deref::operator<< (T& t) const
  {
    return operator<<(const_cast<const T&>(t));
  }


  /*-------------.
  | Standalone.  |
  `-------------*/

  inline
  Deref
  operator<< (std::ostream& ostr, deref_e)
  {
    return Deref (ostr);
  }
}

#endif // !LIBPORT_DEREF_HXX
