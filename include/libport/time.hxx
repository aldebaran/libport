#ifndef LIBPORT_TIME_HXX
# define LIBPORT_TIME_HXX

namespace libport
{
  namespace time
  {
    inline Time
    now()
    {
      return boost::posix_time::microsec_clock::local_time();
    }

    inline Duration
    s(int v)
    {
      return boost::posix_time::seconds(v);
    }

    inline Duration
    ms(int v)
    {
      return boost::posix_time::milliseconds(v);
    }

    inline Duration
    us(int v)
    {
      return boost::posix_time::microseconds(v);
    }

    inline void
    sleep(const Duration& d)
    {
      usleep(d.total_microseconds());
    }
  }
}

#endif
