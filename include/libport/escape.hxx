/**
 ** \file  libport/escape.hxx
 ** \brief Inline methods for libport/escape.hh.
 **/

#ifndef LIBPORT_ESCAPE_HXX
# define LIBPORT_ESCAPE_HXX

# include <libport/escape.hh>
# include <sstream>

namespace libport
{

  template <class T>
  Escape::Escape (const T& obj)
  {
    std::ostringstream os;
    os << obj;
    str_ = os.str ();
  }


  template <class T>
  Escape
  escape(const T& obj)
  {
    return Escape (obj);
  }

  inline
  std::ostream&
  operator<< (std::ostream& o, const Escape& rhs)
  {
    return rhs.print (o);
  }

}

#endif // !LIBPORT_ESCAPE_HXX
