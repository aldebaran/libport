/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#ifndef LIBPORT_UTIME_HXX
# define LIBPORT_UTIME_HXX

namespace libport
{

  template <typename Unit>
  inline
  utime_t
  seconds_to_utime(Unit seconds)
  {
    return seconds * 1000LL * 1000LL;
  }

  inline
  utime_t
  timeval_to_utime(const timeval& t)
  {
    return seconds_to_utime(t.tv_sec) + t.tv_usec;
  }

  inline
  timeval
  utime_to_timeval(utime_t t)
  {
    timeval res;
    res.tv_sec =  t / (1000 * 1000);
    res.tv_usec =  t % (1000 * 1000);
    return res;
  }

}

#endif
