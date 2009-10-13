/*
 * Copyright (C) 2009, Gostai S.A.S.
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
void
skip_if_qemu()
{
  const char* running_qemu = getenv("RUNNING_QEMU");
  if (running_qemu && *running_qemu)
    skip("running under qemu");
}

// Some tests cannot run properly with current versions of WineHQ,
// because they only provide a stub for acceptex.
inline
void
skip_if_wine()
{
  const char* running_wine = getenv("RUNNING_WINE");
  if (running_wine && *running_wine)
    skip("running under Wine");
}

