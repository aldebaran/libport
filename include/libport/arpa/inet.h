/*
 * Copyright (C) 2008, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
