/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cassert>
#include <libport/foreach.hh>
#include <libport/backtrace.hh>

namespace libport
{

  void
  abort()
  {
    foreach(const std::string& str, libport::backtrace())
      std::cerr << str << std::endl;
# ifdef _MSC_VER
    if (getenv("_DEBUG_VCXX"))
      _CrtDbgBreak();
    else
# endif
      std::abort();
  }

}
