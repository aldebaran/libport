#ifndef LIBPORT_UTIME_HH
# define LIBPORT_UTIME_HH

# if defined WIN32

#  include <iostream>
#  ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0400
#  endif
#  include <windows.h>

namespace urbi
{
  inline
  long long
  utime()
  {
    static LARGE_INTEGER pfreq = 0;
    static LARGE_INTEGER base = 0;
    if (pfreq == 0)
    {
      QueryPerformanceFrequency(&pfreq);
      QueryPerformanceCounter(&base);
      std::cerr <<"perfcounter at frequency of " << pfreq << "\n";
    }
    LARGE_INTEGER val;
    QueryPerformanceCounter(&val);
    return (long long) ((val-base) * 1000000LL) / pfreq;
  }
}

# else

#  include <sys/time.h>
#  include <time.h>

namespace urbi
{
  inline
  long long
  utime()
  {
    static long long start = 0;
    static const long long BIGDELTA = 30LL*365LL*24LL*3600LL;
    struct timeval tv;
    gettimeofday(&tv, 0);
    if (start == 0)
      start = (tv.tv_sec-BIGDELTA)*1000000+tv.tv_usec;
    return (tv.tv_sec-BIGDELTA)*1000000+tv.tv_usec - start;
  }
}

# endif
#endif // !LIBPORT_UTIME_HH
