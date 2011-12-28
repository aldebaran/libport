/*
 * Copyright (C) 2007-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_TEST_HH
# define LIBPORT_TEST_HH
#  include <boost/algorithm/string.hpp>
#  include <libport/compiler.hh>
#  include <libport/cstdlib>
#  include <libport/debug.hh>
#  include <libport/format.hh>
#  include <libport/unit-test.hh>
#  include <libport/sysexits.hh>
#  include <iostream>

GD_INIT();

inline
std::string
srcdir()
{
  static std::string res = libport::xgetenv("SRCDIR");
  if (res.empty())
  {
    BOOST_TEST_MESSAGE("SRCDIR is not defined");
    exit(EX_USAGE);
  }
  return res;
}

ATTRIBUTE_NORETURN
void skip(const std::string& env);

inline
void
skip(const std::string& why)
{
  BOOST_TEST_MESSAGE("SKIP: " << why);
  exit(EX_SKIP);
}

// Make no difference between "darwin" and "macos" and "macosx".
inline
std::string
os_normalize(const std::string& s)
{
  if (s == "darwin" || s == "macos" || s == "macosx")
    return "darwin";
  else
    return s;
}

inline
bool
running(const char* env)
{
  std::string val = libport::xgetenv("BUILDFARM_OS");
  BOOST_TEST_MESSAGE("BUILDFARM_OS=" << val);
  return os_normalize(val) == os_normalize(env);
}

inline
void
skip_if(const char* env)
{
  if (running(env))
    skip(libport::format("BUILDFARM_OS=%s", env));
}

#endif // !LIBPORT_TEST_HH
