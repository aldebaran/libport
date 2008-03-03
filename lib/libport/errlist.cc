/**
 ** \file libport/errlist.cc
 ** \brief errlist: implements file libport/_errlist.hh
 */

#include "libport/windows.hh"
#include "libport/bits/errlist.hh"

namespace libport
{

#ifdef WIN32

# define E(code, s) { code, (s " [" #code "]") }

  struct _sys_errlist_t _sys_errlist[] =
  {
    E(WSAEINTR, "Interrupted function call"),
    E(WSAEACCES, "Permission denied"),
    E(WSAEFAULT, "Bad address"),
    E(WSAEINVAL, "Invalid argument"),
    E(WSAEMFILE, "Too many open files"),
    E(WSAEWOULDBLOCK,  "Resource temporarily unavailable"),
    E(WSAEINPROGRESS, "Operation now in progress"),
    E(WSAEALREADY, "Operation already in progress"),
    E(WSAENOTSOCK, "Socket operation on nonsocket"),
    E(WSAEDESTADDRREQ, "Destination address required"),
    E(WSAEMSGSIZE, "Message too long"),
    E(WASEPROTOTYPE, "Protocol wrong for socket"),
    E(WSAENOPROTOOPT, "Bad protocol option"),
    E(WSAEPROTONOSUPPORT, "Protocol not supported"),
    E(WSAESOCKTNOSUPPORT, "Socket type not supported"),
    E(WASEOPNOTSUPP, "Operation not supported"),
    E(WSAEPFNOSUPPORT,  "Protocol family not supported"),
    E(WSAEAFNOSUPPORT, "Address family not supported by protocol family"),
    E(WSAEADDRINUSE, "Address already in use"),
    E(WSAEADDRNOTAVAIL, "Cannot assign requested address"),
    E(WSAENETDOWN, "Network is down"),
    E(WSAENETUNREACH, "Network is unreachable"),
    E(WSAENETRESET, "Network dropped connection on reset")
    E(WSAECONNABORTED, "Software caused connection abort"),
    E(WSAECONNRESET, "Connection reset by peer"),
    E(WSAENOBUFS, "No buffer space avaialable"),
    E(WSAEISCONN, "Socket is already connected"),
    E(WSAENOTCONN, "Socket is not connected"),
    E(WSAESHUTDOWN, "Cannot send after socket shutdown"),
    E(WSAETIMEDOUT, "Connection timed out"),
    E(WSAECONNREFUSED, "Connection refused"),
    E(WSAEHOSTDOWN, "Host is down"),
    E(WSAEHOSTUNREACH, "No route to host"),
    E(WSAEPROCLIM, "Too many processes"),
    E(WSASYSNOTREADY, "Network subsystem is unavailable"),
    E(WSAVERNOTSUPPORTED, "Winsock.dll out of range"),
    E(WSANOTINITIALISED, "Successful WSAStartup not yet performed"),
    E(WSAEDISCONN, "Graceful shutdown no in progress"),
    E(WSATYPE_NOT_FOUND, "Class type not found"),
    E(WSAHOST_NOT_FOUND, "Host not found"),
    E(WSATRY_AGAIN, "Nonauthoritative host not found"),
    E(WSANO_RECOVERY, "This is a nonrecoverable error"),
    E(WSANO_DATA, "Valid name, no data record of requested type)"),
    { -1, NULL},
  };
# undef E

#endif

}
