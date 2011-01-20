/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/debug.hh>
#include <serialize/exception.hh>

GD_CATEGORY(Serialize);

namespace libport
{
  namespace serialize
  {
    Exception::Exception(const std::string& msg)
      : runtime_error(msg)
    {
      GD_FINFO_TRACE("exception: %s", msg);
    }
  }
}
