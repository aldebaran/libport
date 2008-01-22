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

#include <openssl/pem.h>
#include <cassert>
#include "urbi-crypto/xutility.hh"
#include "urbi-crypto/openssl.hh"
#include "urbi-crypto/utility.hh"
#include "urbi-crypto/crypto.hh"

namespace crypto
{


  Crypto::Crypto ()
    : publickey_ (0),
      privatekey_ (0)
  {
  }

  Crypto::~Crypto ()
  {
  }

  void
  Crypto::setPrivateKey(EVP_PKEY* privatekey)
  {
    assert(!!privatekey);
    privatekey_ = privatekey;
  }

  void
  Crypto::setPrivateKey(const char* filename)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Setting private key" << std::endl;
#endif

    FILE *privatekey_file = 0;
    privatekey_file = xfopen(filename, "r+b");
    privatekey_ = PEM_read_PrivateKey(privatekey_file, 0, 0, 0);
    if (!privatekey_)
      THROW_OPENSSL_ERROR(E_READPRIVKEY);
    xfclose(privatekey_file, filename);

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Private key set" << std::endl;
#endif
  }

  void
  Crypto::setPrivateKey(std::string& str)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Setting private key" << std::endl;
#endif

    std::string tmpfile = Utility::createTemporaryFile ();

    /// WRITE THE PUBLIC KEY TO THE TEMPORARY FILE
    // Create temporary file with public key.
    FILE *os = xfopen(tmpfile.c_str (), "wb");
    xfwrite (str.c_str (), sizeof (char), str.size (), os, tmpfile);
    xfclose (os, tmpfile);

    // Load the key.
    setPrivateKey(tmpfile.c_str ());
    Utility::removeTemporaryFile (tmpfile);

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Private key set" << std::endl;
#endif
  }

  void
  Crypto::setPublicKey(EVP_PKEY* publickey)
  {
    assert(publickey);
    publickey_ = publickey;
  }


  void
  Crypto::setPublicKey(const char* filename)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Setting public key -" << std::endl;
#endif

    FILE *publickey_file = xfopen(filename, "r+b");
    publickey_ = PEM_read_PUBKEY(publickey_file, 0, 0, 0);
    if (NULL == publickey_)
      THROW_OPENSSL_ERROR(E_READPUBKEY);

    xfclose(publickey_file, filename);

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Public key set -" << std::endl;
#endif
  }


  void
  Crypto::setPublicKey(std::string& str)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Setting public key" << std::endl;
#endif

    std::string tmpfile = Utility::createTemporaryFile ();

    /// WRITE THE PUBLIC KEY TO THE TEMPORARY FILE
    // Create temporary file with public key.
    FILE *os = xfopen(tmpfile.c_str (), "wb");
    xfwrite (str.c_str (), sizeof (char), str.size (), os, tmpfile);
    xfclose (os, tmpfile);

    // Load the key.
    setPublicKey(tmpfile.c_str ());

    Utility::removeTemporaryFile (tmpfile);

#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** Public key set" << std::endl;
#endif
  }



  ///This function is reading in text mode and removing all newline charachter
  void
  Crypto::setInStream (const char* filename,
		       const char* openmode,
		       const char* stripped)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Appending file - message" << std::endl;
#endif
    Utility::readFileInStream (filename, in_stream_, openmode, stripped);
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** File appended - message" << std::endl;
#endif
  }

  void
  Crypto::setInStream (const char* raw_data, int size)
  {
    xwrite (in_stream_, raw_data, size);
  }

  void
  Crypto::setInStream (std::string& str)
  {
    in_stream_ << str;
  }

  std::string
  Crypto::getInStream () const
  {
    return in_stream_.str ();
  }


  void
  Crypto::setOutStream(const char* filename,
		       const char* openmode,
		       const char* stripped)
  {
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** " << std::endl;
    DEBUG_LOG() << "*** Appending file - signature" << std::endl;
#endif
    Utility::readFileInStream (filename, out_stream_, openmode, stripped);
#ifdef REAL_DEBUG
    DEBUG_LOG() << "*** File appended - signature" << std::endl;
#endif
  }

  void
  Crypto::setOutStream (const char* raw_data, int size)
  {
    xwrite (out_stream_, raw_data, size);
  }


  void
  Crypto::setOutStream(std::string& str)
  {
    out_stream_ << str;
  }

  std::string
  Crypto::getOutStream () const
  {
    return out_stream_.str ();
  }

}
