/**
 ** \file libport/cstdio.cc
 ** \brief Implements file libport/cstdio.
 */

#include <cerrno>
#include <iostream>

#include <libport/cstdio>
#include <libport/cstring>
#include <libport/windows.hh>

namespace libport
{

#ifdef WIN32

  const char*
  getWinErrorMessage()
  {
    static char buf[1024];

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  0, GetLastError(), 0,
                  (LPTSTR)buf, sizeof buf,
                  0);

    return buf;
  }

#endif

  void
  perror(const char* s)
  {
    if (s && *s)
      std::cerr << s << ": ";
    std::cerr << strerror(errno) << std::endl;
  }


  const char*
  strerror(int errnum)
  {
#ifndef WIN32
    return std::strerror(errnum);
#else
    return errnum ? std::strerror(errnum) : getWinErrorMessage();
#endif
  }


}
