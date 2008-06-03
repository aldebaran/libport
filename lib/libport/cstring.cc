/**
 ** \file libport/cstring.cc
 ** \brief string: implements file libport/cstring
 */

#include <boost/lexical_cast.hpp>

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

  const char*
  strerror(int errnum)
  {
#ifndef WIN32
    return ::strerror(errnum);
#else
    return errnum ? ::strerror(errnum) : getWinErrorMessage();
#endif
  }

}
