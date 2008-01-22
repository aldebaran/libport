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

#ifndef URBI_CRYPTO_CRYPTO_HH
# define URBI_CRYPTO_CRYPTO_HH

# include <openssl/evp.h>
# include <string>
# include <sstream>
# include "urbi-crypto/mangle-defines.hh"

namespace crypto
{

  class Crypto
  {
  public:

    Crypto ();

    virtual ~Crypto ();

  public:

    ////////////////////////////////////////////////////////
    ///                                                  ///
    /// Functions to set the RSA/DSA Private/Public keys ///
    ///                                                  ///
    ////////////////////////////////////////////////////////

    void	setPrivateKey (EVP_PKEY* privatekey);

    void	setPrivateKey (const char* filename);

    void	setPrivateKey (std::string& str);

    /// Set a public key to be used to verify the signature of
    /// a file. Take an argument of type 'EVP_PKEY'
    void	setPublicKey (EVP_PKEY* publickey);

    /// Set a public key to be used to verify the signature of
    /// a file. Take a filename as argument. Read it and store it's
    /// content in the public key.
    void	setPublicKey (const char* filename);

    /// Set a public key to be used to verify the signature of
    /// a file. Takes the public key as a string. But the problem
    /// is that we didn't find any functions to set the public key
    /// from a string, so the string will be written in a file, and
    /// then loaded from this file.
    void	setPublicKey (std::string& str);

  public:

    /// Set the stream that will be processed by the algorithm.
    void	setInStream (const char* filename,
			     const char* openmode,
			     const char* stripped);
    void	setInStream (const char* raw_data, int size);
    void	setInStream (std::string& str);
    std::string	getInStream () const;

    /// Set the out stream (this have meaning only for verification)
    void	setOutStream (const char* filename,
			      const char* openmode,
			      const char* stripped);
    void	setOutStream (const char* raw_data, int size);
    void	setOutStream (std::string& str);
    std::string	getOutStream () const;



  protected:
    EVP_PKEY*		publickey_;
    EVP_PKEY*		privatekey_;


    std::stringstream	in_stream_;
    std::stringstream	out_stream_;
  };

}

#endif // !URBI_CRYPTO_CRYPTO_HH
