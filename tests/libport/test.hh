/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <boost/algorithm/string.hpp>
#include <libport/compiler.hh>
#include <libport/cstdlib>
#include <libport/format.hh>
#include <libport/unit-test.hh>
#include <libport/sysexits.hh>
#include <iostream>

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
skip(const std::string& env)
{
  BOOST_TEST_MESSAGE("skipping this test, running " << env);
  exit(EX_SKIP);
}

inline
bool
running(const std::string& env)
{
  const std::string val =
    libport::xgetenv(libport::format("RUNNING_%s", boost::to_upper_copy(env)));
  bool res = !val.empty();
  if (res)
    BOOST_TEST_MESSAGE("Running under " << env);
  return res;
}

inline
void
skip_if(const char* env)
{
  if (running(env))
    skip(env);
}
