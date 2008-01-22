/**
 * File: openssl.hh\n
 * Definition of the openssl class.
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

#include <cassert>

#include <openssl/evp.h>
#include <openssl/err.h>

#include "urbi-crypto/openssl.hh"

namespace crypto
{
  OpenSSL* OpenSSL::instance_ = 0;

  OpenSSL::OpenSSL()
  {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
  }

  OpenSSL::~OpenSSL() throw()
  {
  }

  OpenSSL& OpenSSL::operator= (const OpenSSL&)
  {
    assert(0);
    THROW_CRYPTO("This class can't be copied.");
  }

  void OpenSSL::displayError()
  {
    getInstance();
    ERR_print_errors_fp(stderr);
  }
}
