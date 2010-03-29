/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/sys/time.h
#ifndef LIBPORT_SYS_TIME_H
# define LIBPORT_SYS_TIME_H

# if !defined WIN32 && ! defined LIBPORT_WIN32
#  include <sys/time.h>
# else
#  include <libport/sys/socket.h>
# endif

# include <iostream>

/*----------.
| timeval.  |
`----------*/

namespace std
{
  std::ostream& operator<< (std::ostream& o, const timeval& t);
}

bool operator==(const timeval& lhs, const timeval& rhs);
bool operator!=(const timeval& lhs, const timeval& rhs);

timeval& operator-=(timeval& lhs, const timeval& rhs);

timeval operator-(const timeval& lhs, const timeval& rhs);

# include <libport/sys/time.hxx>

#endif // LIBPORT_SYS_TIME_H
