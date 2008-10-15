/// \file libport/sys/time.h
#ifndef LIBPORT_SYS_TIME_H
# define LIBPORT_SYS_TIME_H

# if !defined WIN32 && ! defined LIBPORT_WIN32
#  include <sys/time.h>
# else
#  include <Winsock2.h>
# endif

# include <iostream>

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

#endif // LIBPORT_SYS_TIME_H
