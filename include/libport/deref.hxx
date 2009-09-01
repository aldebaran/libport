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
  Deref::operator<<(T& t) const
  {
    return ostr_ << t;
  }

  template <typename T>
  inline
  std::ostream&
  Deref::operator<<(T* t) const
  {
    if (t)
      return operator<<(*t);
    else
      return operator<<("NULL");
  }

  /// Bounce the operator<< to handle a \a From to the operator<< for
  /// \a To.
# define BOUNCE(From, To)                       \
  template <typename T>                         \
  inline                                        \
  std::ostream&                                 \
  Deref::operator<<(From) const                 \
  {                                             \
    return operator<<(To);                      \
  }

  BOUNCE(const std::auto_ptr<T>& t, t.get())
  BOUNCE(      std::auto_ptr<T>& t, t.get())
  BOUNCE(const intrusive_ptr<T>& t, t.get())
  BOUNCE(      intrusive_ptr<T>& t, t.get())

# ifndef LIBPORT_NO_BOOST
  BOUNCE(const boost::shared_ptr<T>& t, t.get());
  BOUNCE(      boost::shared_ptr<T>& t, t.get());
# endif

# undef BOUNCE

  /*-------------.
  | Standalone.  |
  `-------------*/

  inline
  Deref
  operator<<(std::ostream& ostr, deref_e)
  {
    return Deref (ostr);
  }
}

#endif // !LIBPORT_DEREF_HXX
