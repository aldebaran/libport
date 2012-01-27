/*
 * Copyright (C) 2008-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>
#include <libport/ctime>
#include <libport/detect-win32.h>
#include <libport/sys/time.h>
#include <libport/utime.hh>

namespace libport
{
  static utime_t reference = 0;

  void
  utime_reference_set(utime_t ref)
  {
    reference = ref;
  }

#if defined WIN32 || defined LIBPORT_WIN32

# include <iostream>
# include <libport/windows.hh>

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
    // NOTE: casting an __int64 in a LARGE_INTEGER is "safe"
    // according to MSDN.
    static __int64 freq = get_performance_frequency();
    __int64 now = get_performance_counter();

    // Beware that this formula might cause an overflow, because of
    // the multiplication before the division: (utime_t) ((now -
    // base) * 1000000LL) / freq; Of course inverting the
    // multiplication and the division is not a solution because of
    // the precision loss.

    // One solution: splitting the operation:
    utime_t high =  (now              / freq) * 1000000LL;
    utime_t low  = ((now * 1000000LL) / freq) % 1000000LL;
    return (high + low) - reference;
  }

#else

# include <libport/ctime>

  utime_t
  utime()
  {
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return utime(t) - reference;
  }
#endif
}
