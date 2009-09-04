/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/compiler.hxx
/// \brief Specific features from some compilers.

namespace libport
{
  inline
  std::ostream&
  operator<<(std::ostream& o, const EchoPrologue& e)
  {
    return e.dump(o);
  }
}
