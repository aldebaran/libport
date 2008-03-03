/**
 ** \file libport/perror.cc
 ** \brief perror: implements file libport/perror.hh
 */

#include <stdio.h>

#include "libport/cstring"

namespace libport
{

  void
  perror (const char* s)
  {
#ifndef WIN32
    ::perror(s);
#else
    int errnum;
    const char* errstring;
    const char* colon;

    errnum = WSAGetLastError();
    errstring = strerror(errnum);

    if (s == NULL || *s == '\0')
      s = colon = "";
    else
      colon = ": ";

    std::cerr << s << colon << errstring << std::endl;
#endif
  }

}
