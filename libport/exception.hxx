/*
 * This file is part of libport
 * Copyright (C) Gostai S.A.S., 2007.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 * For comments, bug reports and feedback: http://www.urbiforge.com
 */

#ifndef EXCEPTION_HXX_
# define EXCEPTION_HXX_

namespace libport
{
  namespace exception
  {
    inline  Exception::Exception (const std::string& thrower, const std::string& msg)
      : std::logic_error (msg), thrower (thrower)
    {};
    
    inline Exception::~Exception () throw (){};
    
    inline Semaphore::Semaphore (const std::string& thrower, const std::string& msg)
      : Exception (thrower, msg)
    {};    
  }
}

#endif  /* !EXCEPTION_HXX_ */
