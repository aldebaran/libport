/**
 ** \file  libport/escape.hxx
 ** \brief Inline methods for libport/escape.hh.
 **/

#ifndef LIBPORT_ESCAPE_HXX
# define LIBPORT_ESCAPE_HXX

# include <sstream>
# include <libport/escape.hh>

namespace libport
{

  template <class T>
  Escape::Escape(const T& obj, char delimiter)
    : delimiter_(delimiter)
  {
    std::ostringstream os;
    os << obj;
    str_ = os.str();
  }

  template <class T>
  Escape
  escape(const T& obj, char delimiter)
  {
    return Escape(obj, delimiter);
  }

  template <class T>
  Escape
  escape(const T& obj)
  {
    return escape(obj, '"');
  }

  inline
  std::ostream&
  operator<< (std::ostream& o, const Escape& rhs)
  {
    return rhs.print(o);
  }

}

#endif // !LIBPORT_ESCAPE_HXX
