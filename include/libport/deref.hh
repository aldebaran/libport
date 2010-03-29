/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file  libport/deref.hh
 ** \brief Automatically derefencing pointers on ostreams.
 **/

#ifndef LIBPORT_DEREF_HH
# define LIBPORT_DEREF_HH

# include <iosfwd>
# include <memory>
# include <libport/export.hh>
# include <libport/intrusive-ptr.hh>
# ifndef LIBPORT_NO_BOOST
#  include <boost/shared_ptr.hpp>
# endif

namespace libport
{
  enum deref_e { deref };

  class LIBPORT_API Deref
  {
  protected:
    Deref (std::ostream&);

  public:
# define LIBPORT_DEREF(Type)                                             \
    template <typename T> std::ostream& operator<< (Type t) const

    // Classes may have an operator<< which is not const.  Support
    // this. In particular, do not bounce non-const to const.

    // Include const and non const cases.
    LIBPORT_DEREF(T*);
    LIBPORT_DEREF(T&);

    LIBPORT_DEREF(const std::auto_ptr<T>&);
    LIBPORT_DEREF(      std::auto_ptr<T>&);
    LIBPORT_DEREF(const intrusive_ptr<T>&);
    LIBPORT_DEREF(      intrusive_ptr<T>&);
# ifndef LIBPORT_NO_BOOST
    LIBPORT_DEREF(const boost::shared_ptr<T>&);
    LIBPORT_DEREF(      boost::shared_ptr<T>&);
# endif
#undef LIBPORT_DEREF

    // As a special case, do not consider that "char*" is to be
    // dereferenced: we want to process it as a whole (a string), and
    // not as a pointer to a single char.
    std::ostream& operator<<(const char* t) const;

  protected:
    friend Deref operator<<(std::ostream&, deref_e);

    std::ostream& ostr_;
  };

  Deref operator<<(std::ostream& ostr, deref_e);

}

# include <libport/deref.hxx>

#endif // !LIBPORT_DEREF_HH
