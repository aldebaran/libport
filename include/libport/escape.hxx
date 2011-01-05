/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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

  inline
  Escape::Escape(const std::string& obj, char delimiter)
  : str_(obj)
  , delimiter_(delimiter)
  {
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
