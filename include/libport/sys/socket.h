/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/sys/socket.h
#ifndef LIBPORT_SYS_SOCKET_H
# define LIBPORT_SYS_SOCKET_H

# include <libport/detect-win32.h>

# if defined WIN32 || defined LIBPORT_WIN32

# include <libport/windows.hh>

/*--------.
| Win32.  |
`--------*/

/* WinSock.h is incompatible with WinSock2.h (and Windows.h includes
 * WinSock.h).  This isn't caught by Windows' headers, so it's caught here.
 */
# ifdef _WINSOCKAPI_
#  error You included WinSock.h (or Windows.h) before this header.
# endif

#  define GROUP __GROUP
#  include <winsock2.h>
#  undef GROUP

# include <libport/unistd.h>

// Define POSIX types if compiling with Visual C++.

// http://msdn.microsoft.com/en-us/library/ms740476(VS.85).aspx
typedef int socklen_t;

#  ifdef _MSC_VER
/* In reality, the MS API says this is a `u_long' although it does not define
 * this type.  See: http://msdn2.microsoft.com/en-us/library/ms738571.aspx  */
typedef UINT32 in_addr_t;
#  elif defined __GNUC__
#   include <cstdint>
typedef uint32_t in_addr_t;
#  else
// Neither VC++ nor GCC: Let's try to continue ...
#  endif

// http://msdn2.microsoft.com/en-us/library/ms740481.aspx
#  define SHUT_RDWR SD_BOTH
#  define SHUT_RD   SD_RECEIVE
#  define SHUT_WR   SD_SEND

# else // !WIN32: Assume UNIX-style headers.

/*---------.
| !Win32.  |
`---------*/

#  include <sys/socket.h>

# endif //! WIN32


// Mac OSX does not have MSG_NOSIGNAL, used by send and recv to ask
// for events to become errno rather than signals.  But it supports
// the socket option SO_NOSIGPIPE.
#if !defined MSG_NOSIGNAL
# define MSG_NOSIGNAL 0
#endif


/* The 4th argument is usually a const void* on UNIX (which makes it
   possible to pass a pointer to int) and a const char* on Windows (in
   which case passing an int* is an error).

   Since we often pass an int*, we really want this wrapper.

   Don't make a macro though, since Boost also defines a setsockopt,
   in which case a macro would break everything.  */
namespace libport
{
  inline
  int
  getsockopt(int s, int level,
             int optname, void* optval, socklen_t* optlen)
  {
    return ::getsockopt(WIN32_IF(static_cast<SOCKET>(s), s),
                        level,
                        optname,
                        WIN32_IF(static_cast<char*>(optval), optval),
                        optlen);
  }

  inline
  int
  setsockopt(int s, int level,
             int optname, const void* optval, socklen_t optlen)
  {
    return ::setsockopt(WIN32_IF(static_cast<SOCKET>(s), s),
                        level,
                        optname,
                        WIN32_IF(static_cast<const char*>(optval), optval),
                        optlen);
  }

}


#endif // !LIBPORT_SYS_SOCKET_H
