/**
 ** \file libport/cstdio.cc
 ** \brief Implements file libport/cstdio.
 */

#include <iostream>

#include <libport/windows.hh>
#include <libport/cstdio>
#include <libport/cstring>

namespace libport
{

  void
  perror (const char* s)
  {
#ifndef WIN32
    ::perror(s);
#else
    if (s && *s)
      std::cerr << s << ": ";
    std::cerr << strerror(errno) << std::endl;
#endif
  }

}
