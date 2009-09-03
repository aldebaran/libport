/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
/**
 ** \file libport/cstdio.cc
 ** \brief Implements file libport/cstdio.
 */

#include <libport/cerrno>
#include <iostream>

#include <libport/cstdio>
#include <libport/cstring>

namespace libport
{

  void
  perror(const char* s)
  {
    if (s && *s)
      std::cerr << s << ": ";
    std::cerr << strerror(errno) << std::endl;
  }

  void
  perror(const std::string& s)
  {
    perror(s.c_str());
  }

  void
  perror(const std::ostringstream& s)
  {
    perror(s.str());
  }

}
