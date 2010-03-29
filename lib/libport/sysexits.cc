/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstdlib>
#include <libport/sysexits.hh>

namespace libport
{
  exit::exit(unsigned s)
    : status_(s)
  {}

  void
  exit::operator() () const
  {
    std::exit (status_);
  }

  std::ostream&
  operator<<(std::ostream&, const exit& e)
  {
    e();
  }

}
