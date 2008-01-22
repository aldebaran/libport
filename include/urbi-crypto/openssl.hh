/**
 * File: openssl.hh\n
 * Definition of the openssl class.
 * Copyright (C) Gostai S.A.S., 2006-2008.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 * For comments, bug reports and feedback: http://www.urbiforge.com
 */

#ifndef LIBCRYPTO_OPENSSL_HH
# define LIBCRYPTO_OPENSSL_HH
# include <openssl/err.h>

# include "urbi-crypto/exception.hh"

# define THROW_OPENSSL()				\
	ERR_print_errors_fp(stderr)

namespace crypto
{
  class OpenSSL
  {
  public:
    static OpenSSL& getInstance()
    {
      if (!instance_)
	instance_ = new OpenSSL();
      return *instance_;
    }

    ~OpenSSL() throw();

    OpenSSL& operator= (const OpenSSL&);

    static void displayError();
  private:
    OpenSSL();

    static  OpenSSL* instance_;
  };
}

#endif /* !LIBCRYPTO_OPENSSL_HH */
