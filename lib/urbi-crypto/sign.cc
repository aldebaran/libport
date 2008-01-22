/**
 * File: sign.cc\n
 * Implementation of the digital signature classes.
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
#include <fstream>
#include <string>
#include <sstream>

#include <openssl/pem.h>

#include "urbi-crypto/openssl.hh"
#include "urbi-crypto/sign.hh"

namespace crypto
{

  Sign::Sign() :
    super_type (),
    context_ ()
  {
  }

  Sign::~Sign()
  {
  }

  Sign& Sign::operator=(const Sign&)
  {
    assert(0);
    THROW_CRYPTO("This class can't be copied.");
  }
}
