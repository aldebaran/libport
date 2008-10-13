#ifndef LIBPORT_NETDB_H
# define LIBPORT_NETDB_H

# include <libport/detect-win32.h>

# if !defined WIN32 && !defined LIBPORT_WIN32
#  include <netdb.h>
# endif

#endif // LIBPORT_NETDB_H
