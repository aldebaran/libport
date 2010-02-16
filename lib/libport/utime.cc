/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>
#include <libport/detect-win32.h>
#include <libport/sys/time.h>
#include <libport/utime.hh>

#if defined WIN32 || defined LIBPORT_WIN32

# include <iostream>
# include <libport/windows.hh>

namespace libport
{

  namespace
  {
    static
    inline
    __int64
    get_performance_counter()
    {
      LARGE_INTEGER res;
      QueryPerformanceCounter(&res);
      return *reinterpret_cast<const __int64*>(&res);
    }

    static
    inline
    __int64
    get_performance_frequency()
    {
      LARGE_INTEGER res;
      QueryPerformanceFrequency(&res);
      return *reinterpret_cast<const __int64*>(&res);
    }

  }

  utime_t
  utime()
  {
    // NOTE: casting an __int64 in a LARGE_INTEGER is "safe" according to MSDN.
    static __int64 freq = get_performance_frequency();
    __int64 now = get_performance_counter();
    static __int64 base = now;
    return (utime_t) ((now - base) * 1000000LL) / freq;
  }
}

#else /* !WIN32 */

# include <libport/ctime>

namespace libport
{
  utime_t
  utime()
  {
    timeval tv;
    gettimeofday(&tv, 0);
    static timeval t0 = tv;
    return timeval_to_utime(tv - t0);
  }
}

#endif // !WIN32
