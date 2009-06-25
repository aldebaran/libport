#include <libport/time.h>

# if defined WIN32

namespace libport
{

  int
  clock_gettime(int, struct timespec* t)
  {
    if (t)
    {
      t->tv_sec = 0;
      t->tv_nsec = 0;
    }
    return 0;
  }

}
#endif // !defined WIN32


