#ifndef LIBPORT_NETWORK_H_
# define LIBPORT_NETWORK_H_

# include "detect_win32.h"

# ifdef WIN32

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

#endif /* !LIBPORT_NETWORK_H_ */
