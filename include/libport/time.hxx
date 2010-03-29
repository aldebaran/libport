/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
