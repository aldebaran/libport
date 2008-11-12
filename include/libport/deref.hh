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
    template <typename T> std::ostream& operator<< (const T* t) const;
    template <typename T> std::ostream& operator<< (T* t) const;

    template <typename T> std::ostream& operator<< (const std::auto_ptr<T>& t) const;

    template <typename T>
    std::ostream& operator<< (const intrusive_ptr<T>& t) const;

# ifndef LIBPORT_NO_BOOST
    template <typename T>
    std::ostream& operator<< (const boost::shared_ptr<T>& t) const;
# endif

    template <typename T> std::ostream& operator<< (const T& t) const;
    template <typename T> std::ostream& operator<< (T& t) const;

  protected:
    friend Deref operator<< (std::ostream&, deref_e);

    std::ostream& ostr_;
  };

  Deref operator<< (std::ostream& ostr, deref_e);

}

# include <libport/deref.hxx>

#endif // !LIBPORT_DEREF_HH
