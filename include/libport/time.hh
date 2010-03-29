/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_TIME_HH
# define LIBPORT_TIME_HH

# include <libport/unistd.h> // Leave unistd.h here because it must be included
                             // before windows.h, and thus before
                             // boost/date_time/*.
# include <boost/date_time/posix_time/posix_time.hpp>

namespace libport
{
  typedef boost::posix_time::ptime Time;
  typedef boost::posix_time::time_duration Duration;

  namespace time
  {
    Time now();
    Duration s(int);
    Duration ms(int);
    Duration us(int);
    Duration ns(int);
    void sleep(const Duration& time);
  }
}

# include <libport/time.hxx>

#endif
