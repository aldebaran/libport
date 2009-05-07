#ifndef LIBPORT_UTIME_HXX
# define LIBPORT_UTIME_HXX

namespace libport
{
  inline
  utime_t
  seconds_to_utime(unsigned long seconds)
  {
    return seconds * 1000 * 1000;
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
