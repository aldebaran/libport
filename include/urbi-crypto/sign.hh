/**
 * File: sign.hh\n
 * Definition of the digital signature classes.
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

#ifndef LIBCRYPTO_SIGN_HH
# define LIBCRYPTO_SIGN_HH

# include <openssl/evp.h>
# include "urbi-crypto/crypto.hh"

namespace crypto
{

  class Sign : public Crypto
  {
    typedef Crypto super_type;

  public:

    Sign();

    virtual ~Sign ();

    Sign& operator=(const Sign&);

    virtual void sign () = 0;
    virtual bool check () = 0;

  protected:
    EVP_MD_CTX		context_;

  };
}

#endif /* !LIBCRYPTO_SIGN_HH */
