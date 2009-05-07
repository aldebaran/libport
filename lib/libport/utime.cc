#include <libport/config.h>
#include <libport/detect-win32.h>
#include <libport/sys/time.h>
#include <libport/utime.hh>

#if defined WIN32 || defined LIBPORT_WIN32

# include <iostream>
# include <libport/windows.hh>

namespace libport
{
  utime_t
  utime()
  {
    // NOTE: casting an __int64 in a LARGE_INTEGER is "safe" according to MSDN.
    static __int64 pfreq = 0;
    static __int64 base = 0;

    if (!pfreq)
    {
      QueryPerformanceFrequency((LARGE_INTEGER*) &pfreq);
      QueryPerformanceCounter ((LARGE_INTEGER*) &base);
      // std::cerr << "perfcounter at frequency of " << pfreq << "\n";
    }
    __int64 val;
    QueryPerformanceCounter((LARGE_INTEGER*) &val);
    return (utime_t) ((val - base) * 1000000LL) / pfreq;
  }
}

#else /* !WIN32 */

# include <time.h>

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
