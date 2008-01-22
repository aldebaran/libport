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

#include <cassert>
#include <openssl/aes.h>
#include <iostream>
#include <ios>
#include "urbi-crypto/openssl.hh"
#include "urbi-crypto/xutility.hh"
#include "urbi-crypto/exception.hh"
#include "urbi-crypto/cryptaes.hh"


namespace crypto
{


  CryptAES::CryptAES ()
    : super_type (),
      iv_ (0),
      key_ (0)
  {}

  CryptAES::~CryptAES () throw ()
  {
    delete[] iv_;
    delete[] key_;
  }

  void
  CryptAES::crypt ()
  {
    /// FIXME: implement this function
    assert (0);
  }

  void
  CryptAES::decrypt ()
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Decrypting with AES" << std::endl;
#endif

    AES_KEY aesKey, *key = &aesKey;
    AES_set_decrypt_key((const unsigned char*) key_, 16 * 8, key);

    // get length of file:
    xseekg (in_stream_, 0, std::ios::end);
    int size = xtellg(in_stream_);
    xseekg (in_stream_, 0, std::ios::beg);

    int i;
    unsigned char *in = new unsigned char[size];
    xread (in_stream_, (char*) in, size);

    char out[AES_BLOCK_SIZE];
    if (size <= AES_BLOCK_SIZE)
    {
      // Decrypt part following initialisation vector
      AES_decrypt(in, (unsigned char*) out, key);

      // xor the file bytes with the IV bytes
      for (i = 0; i < size; ++i)
	out[i] ^= iv_[i];

      xwrite (out_stream_, out, size);
    }
    // Else, size is larger than block-size and we need to do CBC chaining
    else
    {
      int cLen = 0;
      for ( ; cLen < size; )
      {
	// Decrypt the next block in the chain
	AES_decrypt(&in[cLen], (unsigned char*) out, key);

	// CBC unchain the previous ciphertext block
	if (cLen < AES_BLOCK_SIZE) /// 1st block
	  for (i=0; i < AES_BLOCK_SIZE; i++)
	    out[i] ^= iv_[i];
	else
	  for (i=0; i < AES_BLOCK_SIZE; i++)
	    out[i] ^= in[i + cLen - AES_BLOCK_SIZE];

	cLen += AES_BLOCK_SIZE;
	xwrite (out_stream_, out, AES_BLOCK_SIZE);
      }
    }
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Decrypting done" << std::endl;
#endif
  }

  void
  CryptAES::setInitVector (const char *iv, int size)
  {
    if (iv_)
      delete[] iv_;
    iv_ = new char[size];
    memcpy (iv_, iv, size);
  }

  void
  CryptAES::setKey (const char* key, int size)
  {
    if (key_)
      delete[] key_;
    key_ = new char[size];
    memcpy (key_, key, size);
  }

}
