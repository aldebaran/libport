/**
 * File: crypt.cc\n
 * Implementation of the crypting classes.
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
#include "urbi-crypto/exception.hh"
#include "urbi-crypto/crypt.hh"

namespace crypto
{

  Crypt::Crypt() :
    super_type ()
  {
  }

  Crypt::~Crypt()
  {
  }

  Crypt& Crypt::operator=(const Crypt&)
  {
    assert(0);
    THROW_CRYPTO("This class can't be copied.");
  }
}
