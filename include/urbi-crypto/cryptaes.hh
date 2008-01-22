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

#ifndef CRYPT_AES_HH_
# define CRYPT_AES_HH_

# include "urbi-crypto/crypt.hh"


namespace crypto
{

  class CryptAES : public Crypt
  {
    typedef Crypt super_type;

  public:

    CryptAES ();

    ~CryptAES () throw ();

    virtual void	crypt ();

    /// Decrypt the message contained in inStream. Call
    /// 'getOutStream ()' after calling 'decrypt ()' to
    /// get the initial message. This message might have
    /// been padded, so it's possible you find some extra '0'
    /// at the end of the message.
    virtual void	decrypt ();

  public:

    /// Set the initialisation vector (iv) for AES
    void		setInitVector (const char *iv, int size);

    /// Set the key for AES
    void		setKey (const char* key, int size);

  protected:

    char*		iv_;
    char*		key_;
  };

}

#endif
