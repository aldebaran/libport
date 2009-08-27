/**
 ** \file libport/cstring.cc
 ** \brief Implements file libport/cstring.
 */

#include <libport/cstring>
#include <libport/windows.hh>

namespace libport
{

#ifdef WIN32

  static const char*
  getWinErrorMessage(DWORD err = GetLastError())
  {
    static char buf[1024];

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  0, err, 0,
                  (LPTSTR)buf, sizeof buf,
                  0);

    return buf;
  }

#endif

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
