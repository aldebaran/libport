/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
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
#include <libport/cstdlib>
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

#if defined WIN32
int asprintf(char **strp, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  int res = vasprintf(strp, fmt, ap);
  va_end(ap);
  return res;
}

int vasprintf(char **strp, const char *fmt, va_list ap)
{
  static const size_t size = 1024; // 1k should be enough for everybody
  *strp = reinterpret_cast<char*>(malloc(size * sizeof(**strp)));
  return vsnprintf(*strp, size, fmt, ap);
}
#endif
