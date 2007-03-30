#ifndef LIBPORT_NETWORK_H_
# define LIBPORT_NETWORK_H_

# include "detect_win32.h"

# if defined WIN32 || defined LIBPORT_WIN32

/* WinSock.h is incompatible with WinSock2.h (and Windows.h includes
 * WinSock.h).  This isn't caught by Windows' headers, so it's caught here.
 */
# ifdef _WINSOCKAPI_
#  error You included WinSock.h (or Windows.h) before this header.
# endif

#  define GROUP __GROUP
#  include <WinSock2.h>
#  undef GROUP

// Define POSIX types if compiling with Visual C++.
#  ifdef _MSC_VER
typedef UINT32 socklen_t;
/* In reality, the MS API says this is a `u_long' although it does not define
 * this type.  See: http://msdn2.microsoft.com/en-us/library/ms738571.aspx
 */
typedef UINT32 in_addr_t;
#  endif

// http://msdn2.microsoft.com/en-us/library/ms740481.aspx
#  define SHUT_RDWR SD_BOTH
#  define SHUT_RD SD_RECEIVE
#  define SHUT_WR SD_SEND

/* The 4th argument is usually a const void* on UNIX and a const char* on
 * Windows.  */
#  define setsockopt(Fd, Lvl, Optname, Optval, Optlen) \
	  setsockopt((Fd), (Lvl), (Optname), (const char*) (Optval), (Optlen))

# else /* !WIN32: Assume UNIX-style headers for networking.  */

#  include <sys/types.h>
#  include <sys/socket.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <netinet/in.h>
#  include <sys/select.h>

# endif /* WIN32 */


/*---------.
| fd_set.  |
`---------*/

# ifdef WIN32
// On windows, file descriptors are defined as u_int (i.e., unsigned int).
#  define LIBPORT_FD_SET(N, P) FD_SET(static_cast<u_int>(N), P)
# else
#  define LIBPORT_FD_SET(N, P) FD_SET(N, P)
# endif

// On MingW, using "mingw32-gcc.exe (GCC) 3.4.5 (mingw special)", it
// seems that FD_ISSSET casts its argument as "fd_set*", without
// const.
# define LIBPORT_FD_ISSET(I, S) FD_ISSET(I, const_cast<fd_set*> (S))


# include <iostream>

namespace std
{
  inline
  std::ostream&
  operator<< (std::ostream& o, const fd_set& s)
  {
    o << "fd_set {";
    bool not_first = false;
    for (int i = 0; i < FD_SETSIZE; ++i)
      if (LIBPORT_FD_ISSET(i, &s))
	o << (not_first++ ? ", " : "") << i;
    return o << " }";
  }
}


/*----------.
| timeval.  |
`----------*/

namespace std
{
  inline
  std::ostream&
  operator<< (std::ostream& o, const timeval& t)
  {
    return o
      << "time_val"
      << "{ tv_sec = " << t.tv_sec
      << ", tv_usec = " << t.tv_usec
      << " }";
  }
}

#endif /* !LIBPORT_NETWORK_H_ */
