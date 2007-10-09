/**
 ** \file  libport/deref.hh
 ** \brief Automatically derefencing pointers on ostreams.
 **/

#ifndef LIBPORT_DEREF_HH
# define LIBPORT_DEREF_HH

# include <iosfwd>
# include "libport/shared-ptr.hh"

namespace libport
{
  enum deref_e { deref };

  class Deref
  {
  protected:
    Deref (std::ostream&);

  public:
    template <typename T> std::ostream& operator<< (const T* t) const;
    template <typename T> std::ostream& operator<< (T* t) const;
# ifndef LIBPORT_NO_BOOST
    template <typename T> std::ostream& operator<< (const shared_ptr<T>& t) const;
# endif // !LIBPORT_NO_BOOST
    template <typename T> std::ostream& operator<< (const T& t) const;
    template <typename T> std::ostream& operator<< (T& t) const;

  protected:
    friend Deref operator<< (std::ostream&, deref_e);

    std::ostream& ostr_;
  };

  Deref operator<< (std::ostream& ostr, deref_e);

}

# include "libport/deref.hxx"

#endif // !LIBPORT_DEREF_HH
