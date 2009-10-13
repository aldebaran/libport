/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
