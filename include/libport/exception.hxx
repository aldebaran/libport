/*
 * This file is part of libport
 * Copyright (C) 2007, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 * For comments, bug reports and feedback: http://www.urbiforge.com
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

    inline const char*
    Exception::what() const throw ()
    {
      return msg_.c_str();
    }
  }
}

#endif // !LIBPORT_EXCEPTION_HXX
