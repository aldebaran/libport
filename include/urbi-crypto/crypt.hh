/**
 * File: crypt.hh\n
 * Definition of the crypting classes.
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

#ifndef CRYPT_HH_
# define CRYPT_HH_

# include "urbi-crypto/crypto.hh"

namespace crypto
{

  class Crypt : public Crypto
  {
    typedef Crypto super_type;

  public:

    Crypt ();

    virtual ~Crypt ();

    Crypt& operator=(const Crypt&);

    /// Overload this function in subclasses to define the
    /// crypting method
    virtual void crypt () = 0;

    /// Overload this function in subclasses to define the
    /// de-crypting method
    virtual void decrypt () = 0;

  };

}

#endif
