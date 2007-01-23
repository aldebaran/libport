#ifndef LIBPORT_NETWORK_H_
# define LIBPORT_NETWORK_H_

# include "detect_win32.h"

# if defined WIN32 || defined LIBPORT_WIN32

/* WinSock.h is incompatible with WinSock2.h (and Windows.h includes
 * WinSock.h). This isn't caught by Windows' headers, so it's caught here.
 */
# ifdef _WINSOCKAPI_
#  error You included WinSock.h (or Windows.h) before this header.
# endif

#  define GROUP __GROUP
#  include <WinSock2.h>
#  undef GROUP

typedef int socklen_t;

# else /* !WIN32: Assume UNIX-style headers for networking. */

#  include <sys/types.h>
#  include <sys/socket.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <netinet/in.h>
#  include <sys/select.h>

# endif /* WIN32 */

// In the long run, this should be part of libport, but I don't know
// where actually :( Should it be libport/sys/types.hh?  Or unistd.hh?
// What standard header should do that?

#ifdef WIN32
// On windows, file descriptors are defined as u_int (i.e., unsigned int).
# define LIBPORT_FD_SET(N, P) FD_SET(static_cast<u_int>(N), P)
#else
# define LIBPORT_FD_SET(N, P) FD_SET(N, P)
#endif

#endif /* !LIBPORT_NETWORK_H_ */
