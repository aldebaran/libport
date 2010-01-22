/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/compiler.hh>
#include <libport/cstdlib>
#include <libport/unit-test.hh>
#include <libport/sysexits.hh>
#include <iostream>

inline
std::string
srcdir()
{
  static std::string res = libport::xgetenv("SRCDIR");
  if (res.empty())
    std::cerr << "SRCDIR is not defined" << std::endl
              << libport::exit(EX_USAGE);
  return res;
}

ATTRIBUTE_NORETURN
void skip(const std::string& why);

inline
void
skip(const std::string& why)
{
  std::cerr << "skipping this test: " << why << std::endl
            << libport::exit(EX_SKIP);
}

inline
bool
running(const char* envvar, const char* message)
{
  const char* cp = getenv(envvar);
  bool res = cp && *cp;
  if (res)
    std::cerr << message << std::endl;
  return res;
}

inline
void
skip_if(const var* envvar, const char* message)
{
  if (skip(envvar, message)
    skip(message);
}

inline
void
skip_if_qemu()
{
  skip("RUNNING_QEMU", "running under qemu");
}

// Some tests cannot run properly with current versions of WineHQ,
// because they only provide a stub for acceptex.
inline
void
skip_if_wine()
{
  skip("RUNNING_WINE", "running under qemu");
}

