#ifndef LIBPORT_TIME_H
# define LIBPORT_TIME_H

# include <libport/detect-win32.h>
# include <libport/export.hh>

# if defined WIN32
#  include <libport/windows.hh>

#  define CLOCK_REALTIME 0

namespace libport
{

  /*--------------------------.
  | FIXME: Why in libport???  |
  `--------------------------*/

  typedef unsigned int time_t;

  struct timespec
  {
    time_t tv_sec;
    long tv_nsec;
  };

  LIBPORT_API
  int clock_gettime(int, struct timespec* t);
}

// Warning: this implementation is not thread safe.
LIBPORT_API
struct tm *
localtime_r(const time_t *clock, struct tm *result);

# endif

#endif // ! LIBPORT_TIME_H
