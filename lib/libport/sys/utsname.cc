/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/sys/utsname.h>
#include <libport/cassert>

namespace libport
{

  utsname::utsname()
  {
    ERRNO_RUN(uname, &utsname_);
  }

#define DEFINE(Function, Member)                \
  std::string                                   \
  utsname::Function() const                     \
  {                                             \
    return utsname_.Member;                     \
  }

  DEFINE(machine, machine);
  DEFINE(node,    nodename);
  DEFINE(release, release);
  DEFINE(system,  sysname);
  DEFINE(version, version);
#undef DEFINE

}
