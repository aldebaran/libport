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

#include "urbi-crypto/xutility.hh"
#include "urbi-crypto/signsha1.hh"
#include <iostream>

namespace crypto
{

  SignSHA1::SignSHA1()
    : super_type ()
  {
  }

  SignSHA1::~SignSHA1() throw()
  {
  }

  SignSHA1&
  SignSHA1::operator= (const SignSHA1&)
  {
    assert(0);
    THROW_CRYPTO("This class can't be copied.");
  }

  void
  SignSHA1::sign()
  {
    unsigned siglen;
    if (!privatekey_)
      THROW_CRYPTO(E_NOPRIVATEKEY);

    if (!EVP_SignInit(&context_, EVP_sha1()))
      THROW_OPENSSL_ERROR(E_SIGNINIT);

    if (!EVP_SignUpdate(&context_,
			in_stream_.str().c_str(),
			in_stream_.str().size()))
      THROW_OPENSSL_ERROR(E_SIGNUPDATE);

    unsigned l = EVP_PKEY_size(privatekey_);

    unsigned char* res = new unsigned char[l+1];
    if (!EVP_SignFinal(&context_, res, &siglen, privatekey_))
      THROW_OPENSSL_ERROR(E_SIGNFINAL);
    res[l] = 0;
    xwrite (out_stream_, (const char*) res, l);
    delete[] res;
  }

  bool
  SignSHA1::check()
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Checking signature" << std::endl;
#endif

    if (!publickey_)
      THROW_CRYPTO(E_NOPUBLICKEY);

    if (!EVP_VerifyInit(&context_, EVP_sha1()))
      THROW_OPENSSL_ERROR(E_VERIFYINIT);

    if (!EVP_VerifyUpdate(&context_,
			  in_stream_.str().c_str(),
			  in_stream_.str().size()))
      THROW_OPENSSL_ERROR(E_VERIFYUPDATE);

    int ret = EVP_VerifyFinal(&context_,
			      (unsigned char*)(out_stream_.str().c_str()),
			      out_stream_.str().size(),
			      publickey_);

    if (ret <= 0)
      THROW_OPENSSL_ERROR(E_VERIFYFINAL);

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Signature checked, signature is"
		<< (ret ? " valid" : " invalid")
		<< std::endl;
#endif

    if (ret == 1)
      return true;
    return false;
  }
}
