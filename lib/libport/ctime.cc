/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/ctime>

# if defined WIN32

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

struct tm *
localtime_r(const time_t *clock, struct tm *res)
{
  if (res)
    *res = *localtime(clock);
  return res;
}

#endif // !defined WIN32
