/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
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


