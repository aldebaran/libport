/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/sys/select.h

#ifndef LIBPORT_SYS_SELECT_H
# define LIBPORT_SYS_SELECT_H

# include <libport/detect-win32.h>

# ifndef WIN32
#  include <sys/select.h>
# else
#  include <libport/sys/socket.h>
# endif
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
    bool first = true;
# ifndef WIN32
    for (int i = 0; i < FD_SETSIZE; ++i)
      if (LIBPORT_FD_ISSET(i, &s))
      {
        o << (first ? "" : ", ") << i;
        first = false;
      }
# else
    for (unsigned int i = 0; i < s.fd_count; ++i)
    {
      o << (first ? "" : ", ") << s.fd_array[i];
      first = false;
    }
# endif
    return o << " }";
  }
}

#endif // !LIBPORT_SYS_SELECT_H
