/*
 * Copyright (C) Gostai S.A.S., 2007.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 * For comments, bug reports and feedback: http://www.urbiforge.com
 */

#ifndef LIBPORT_EXCEPTION_HH
# define LIBPORT_EXCEPTION_HH

# include <stdexcept>

namespace libport
{
  namespace exception
  {
    /// Interface for all exceptions thrown by the libfsm.
    struct Exception: public std::logic_error
    {
      /** Ctor.
       *  @param  thrower  Name of function/method throwing the exception.
       *  @param  msg      Reason for throwing the exception.
       */
      Exception (const std::string& thrower, const std::string& msg);
      virtual ~Exception () throw ();
      std::string thrower;
    };

    /// Exception.
    struct  Semaphore : public Exception
    {
      Semaphore (const std::string& thrower, const std::string& msg);
    };
  }
}

# include "libport/exception.hxx"

#endif // !LIBPORT_EXCEPTION_HH
