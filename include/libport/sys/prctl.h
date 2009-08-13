#ifndef LIBPORT_SYS_PRCTL_H
# define LIBPORT_SYS_PRCTL_H

# include <libport/config.h>

# ifdef LIBPORT_HAVE_SYS_PRCTL_H
#  include <sys/prctl.h>

# else // !LIBPORT_HAVE_SYS_PRCTL_H

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
