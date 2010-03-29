/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_EXCEPTION_HXX
# define LIBPORT_EXCEPTION_HXX

namespace libport
{
  namespace exception
  {
    inline
    Exception::Exception(const std::string& msg)
      : msg_(msg)
    {}

    inline
    Exception::~Exception() throw ()
    {}

    inline
    const char*
    Exception::what() const throw ()
    {
      return msg_.c_str();
    }

    inline
    const std::string&
    Exception::message() const throw ()
    {
      return msg_;
    }

    inline
    std::ostream&
    operator<<(std::ostream& o, const Exception& e)
    {
      return o << e.message();
    }

  }
}

#endif // !LIBPORT_EXCEPTION_HXX
