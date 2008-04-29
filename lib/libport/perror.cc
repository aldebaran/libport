/**
 ** \file libport/perror.cc
 ** \brief perror: implements file libport/perror.hh
 */

#include <iostream>

#include "libport/windows.hh"
#include "libport/cstdio"
#include "libport/cstring"

namespace libport
{

  void
  perror (const char* s)
  {
#ifndef WIN32
    ::perror(s);
#else
    if (errno != 0)
      ::perror(s);
    else
    {
      if (s && *s)
        std::cerr << s << ": ";
      std::cerr << getWinErrorMessage() << std::endl;
    }
#endif
  }

}
