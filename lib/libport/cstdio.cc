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
