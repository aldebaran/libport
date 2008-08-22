#include <libport/config.h>
#include <libport/detect-win32.h>
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

# include <sys/time.h>
# include <time.h>

namespace libport
{
  utime_t
  utime()
  {
    static utime_t start = 0;
    static const utime_t BIGDELTA = 30LL * 365LL * 24LL * 3600LL;
    struct timeval tv;
    gettimeofday(&tv, 0);
    if (start == 0)
      start = (tv.tv_sec - BIGDELTA) * 1000000LL + tv.tv_usec;
    return (tv.tv_sec - BIGDELTA) * 1000000LL + tv.tv_usec - start;
  }
}

#endif // !WIN32
