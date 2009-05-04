#ifndef LIBPORT_UTIME_HXX
# define LIBPORT_UTIME_HXX

namespace libport
{
  inline
  struct timeval
  utime_to_timeval(utime_t t)
  {
    struct timeval res;
    res.tv_sec =  t / (1000 * 1000);
    res.tv_usec =  t % (1000 * 1000) * (1000 * 1000);
    return res;
  }
}

#endif
