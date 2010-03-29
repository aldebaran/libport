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
