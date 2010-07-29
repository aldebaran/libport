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
#include <sstream>

namespace libport
{

  utsname::utsname()
    : major_(0)
    , minor_(0)
    , patchlevel_(0)
  {
    ERRNO_RUN(uname, &utsname_);
    std::stringstream r(release());
    r >> major_;
    assert_eq(r.get(), '.');
    r >> minor_;
    assert_eq(r.get(), '.');
    r >> patchlevel_;
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

#define DEFINE(Function)                        \
  unsigned                                      \
  utsname::release_ ## Function() const         \
  {                                             \
    return Function ## _;                       \
  }

  DEFINE(major);
  DEFINE(minor);
  DEFINE(patchlevel);
#undef DEFINE

}
