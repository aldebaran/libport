/**
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

#include <iostream>
#include <cassert>
#include <openssl/pem.h>
#include "urbi-crypto/openssl.hh"
#include "urbi-crypto/xutility.hh"
#include "urbi-crypto/exception.hh"
#include "urbi-crypto/cryptrsa.hh"

namespace crypto
{

  CryptRSA::CryptRSA ()
    : super_type ()
  {
  }

  CryptRSA::~CryptRSA () throw ()
  {
  }

  void
  CryptRSA::crypt ()
  {
    assert (0); /// FIXME: implement this function
  }

  void
  CryptRSA::decrypt ()
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Decrypting with RSA" << std::endl;
#endif
    if (!privatekey_)
      THROW_CRYPTO(E_NOPRIVATEKEY);

    RSA* rsa_private_key = EVP_PKEY_get1_RSA(privatekey_);

    if (!rsa_private_key)
      THROW_OPENSSL_ERROR(E_PKEYTORSA);

    int ret = 0;
    int l = RSA_size(rsa_private_key);

    unsigned char* res = new unsigned char[l+1];
    if (-1 == (ret = RSA_private_decrypt(in_stream_.str ().length (),
					 (const unsigned char*) in_stream_.str ().c_str (),
					 res,
					 rsa_private_key,
					 RSA_PKCS1_PADDING)))
      THROW_OPENSSL_ERROR (E_PRIVDECRYPT);
    res[ret] = 0;
    xwrite (out_stream_, (const char*) res, ret);
    delete[] res;
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Decrypting done" << std::endl;
#endif
  }

}
