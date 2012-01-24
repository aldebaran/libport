/*
 * Copyright (C) 2009-2010, 2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/sys/types.h>

namespace libport
{

  template <typename Unit>
  inline
  utime_t
  seconds_to_utime(Unit seconds)
  {
    return utime_t(seconds * 1000LL * 1000LL);
  }

  inline
  utime_t
  utime(const timespec& t)
  {
    return seconds_to_utime(t.tv_sec) + t.tv_nsec / 1000;
  }

  inline
  utime_t
  utime(const timeval& t)
  {
    return seconds_to_utime(t.tv_sec) + t.tv_usec;
  }

  inline
  timeval
  utime_to_timeval(utime_t t)
  {
    timeval res;
    // Explicit convertions to silence MSVC warnings.
    res.tv_sec = long(t / (1000 * 1000));
    res.tv_usec = suseconds_t(t % (1000 * 1000));
    return res;
  }

}
