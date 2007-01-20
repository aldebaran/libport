#ifndef LIBPORT_UTIME_HH
# define LIBPORT_UTIME_HH

# if defined WIN32 || defined LIBPORT_WIN32

#  include <iostream>
#  include "libport/windows.hh"

namespace libport
{
  inline
  long long
  utime()
  {
    // NOTE: casting an __int64 in a LARGE_INTEGER is "safe" according to MSDN.
    static __int64 pfreq = 0;
    static __int64 base = 0;

    if (pfreq == 0)
    {
      QueryPerformanceFrequency((LARGE_INTEGER*) &pfreq);
      QueryPerformanceCounter ((LARGE_INTEGER*) &base);
      // std::cerr << "perfcounter at frequency of " << pfreq << "\n";
    }
    __int64 val;
    QueryPerformanceCounter((LARGE_INTEGER*) &val);
    return (long long) ((val - base) * 1000000LL) / pfreq;
  }
}

# else /* !WIN32 */

#  include <sys/time.h>
#  include <time.h>

namespace libport
{
  inline
  long long
  utime()
  {
    static long long start = 0;
    static const long long BIGDELTA = 30LL * 365LL * 24LL * 3600LL;
    struct timeval tv;
    gettimeofday(&tv, 0);
    if (start == 0)
      start = (tv.tv_sec - BIGDELTA) * 1000000LL + tv.tv_usec;
    return (tv.tv_sec - BIGDELTA) * 1000000LL + tv.tv_usec - start;
  }
}

# endif
#endif // !LIBPORT_UTIME_HH
