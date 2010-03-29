/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SYS_PRCTL_H
# define LIBPORT_SYS_PRCTL_H

# include <libport/config.h>

# ifdef LIBPORT_HAVE_SYS_PRCTL_H
#  include <sys/prctl.h>

# else // !LIBPORT_HAVE_SYS_PRCTL_H

#  include <libport/cerrno>
#  define PR_SET_PDEATHSIG 0

extern "C"
{
  inline
  int prctl(int /* option */, ...)
  {
    errno = ENOTSUP;
    return -1;
  }
}

# endif // !LIBPORT_HAVE_SYS_PRCTL_H

#endif // !LIBPORT_SYS_PRCTL_H
