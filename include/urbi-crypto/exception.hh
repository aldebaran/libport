/**
 * File: exception.hh\n
 * Definition of the crypto exception.
 *
 * Copyright (C) Gostai S.A.S., 2006-2008.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 * For comments, bug reports and feedback: http://www.urbiforge.com
 */

#ifndef LIBCRYPTO_EXCEPTION_HH
# define LIBCRYPTO_EXCEPTION_HH
# include <exception>
# include <string>
# include <sstream>
# include <fstream>
# include <errno.h>
# include "urbi-crypto/debug-log.hh"

# define THROW_CRYPTO(x)							\
{										\
  if (::crypto::DebugLog::instance ().logEnabled ())				\
    ::crypto::DebugLog::instance ().out () << "Error: " << x << std:: endl;	\
  throw ::crypto::Exception(x, __FILE__, __LINE__);				\
}

# define THROW_OPENSSL_ERROR(x)			\
{						\
  std::string msg(x);				\
  while (unsigned long code = ERR_get_error())	\
    msg += ERR_error_string(code, NULL);	\
  THROW_CRYPTO(msg.c_str ());			\
}


extern int errno;

#define THROW_FILE_ERROR_AND_INCLUDE_ERNO_MSG(custom_msg, filename)	\
  {									\
    std::string error_msg__ (custom_msg);				\
    error_msg__ += ": ";						\
    error_msg__ += filename;						\
    error_msg__ += ": ";						\
    char* errno_error_msg__ = strerror (errno);				\
    if (errno_error_msg__)						\
      error_msg__ += errno_error_msg__;					\
    THROW_CRYPTO (error_msg__.c_str ());				\
  }

namespace crypto
{
  class Exception : public std::exception
  {
  public:
    Exception (const char* msg, const char* file, int line)
    {
      std::ostringstream oss;
      oss << file << ":" << line << ": " << msg;
      msg_ = oss.str();
    }

    virtual ~Exception () throw()
    {
    }
    virtual const char* what () const throw()
    {
      return this->msg_.c_str();
    }
  private:
    std::string msg_;
  };
}

#endif /* !LIBCRYPTO_EXCEPTION_HH */
