/**
 ** \file libport/string.cc
 ** \brief string: implements file libport/string.hh
 */

#include <boost/lexical_cast.hpp>

#include "libport/cstring"

#include "libport/detect_win32.h"

namespace libport
{

#ifdef WIN32

# define LIBPORT_BUFFER_SIZE  1024
  const char*
  getWinErrorMessage()
  {
    static char msg_buf[LIBPORT_BUFFER_SIZE];

    FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
                   | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0,
                   (LPTSTR)msg_buf, LIBPORT_BUFFER_SIZE, NULL);

    return msg_buf;
  }
# undef LIBPORT_BUFFER_SIZE

#endif

  const char*
  strerror(int errnum)
  {
#ifndef WIN32
    return ::strerror(errnum);
#else
    const char* str;

    if (errnum != 0)
      str = ::strerror(errnum);
    else
      str = getWinErrorMessage();

    return str;
#endif
  }

}
