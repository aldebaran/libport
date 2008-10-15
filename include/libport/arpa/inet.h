#ifndef LIBPORT_ARPA_INET_H
# define LIBPORT_ARPA_INET_H

# include <libport/detect-win32.h>

# if !defined WIN32 && !defined LIBPORT_WIN32
#  include <arpa/inet.h>
# else
#  include <libport/sys/socket.h>
#  include <ws2tcpip.h>
# endif

#endif // LIBPORT_ARPA_INET_H
