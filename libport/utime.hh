#ifndef LIBPORT_UTIME_HH
# define LIBPORT_UTIME_HH

# if defined WIN32
namespace urbi
{
  inline 
  long long
  utime()
  {
    static long long pfreq= 0LL; //cps
    static long long base = 0LL;
    if (pfreq == 0)
      {
	QueryPerformanceFrequency((LARGE_INTEGER *)&pfreq);
	QueryPerformanceCounter((LARGE_INTEGER *)&base);
	std::cerr <<"perfcounter at frequency of "<<pfreq<<"\n";
      }
    long long val;
    QueryPerformanceCounter((LARGE_INTEGER *)&val);
    return ((val-base) * 1000000LL)/ pfreq;
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
