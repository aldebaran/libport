/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/*
 * \file libport/deref.cc
 * \brief Implementation for libport/deref.hh.
 */

#include <libport/deref.hh>

namespace libport
{

  Deref::Deref (std::ostream& o)
    : ostr_(o)
  {
  }

  std::ostream&
  Deref::operator<< (const char* t) const
  {
    return ostr_ << (t ? t : "NULL");
  }

}
