/*
 * Copyright (C) 2009-2010, 2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/ctime>

#if defined __APPLE__
# include <libport/cerrno>
// https://gist.github.com/1087739
# include <mach/clock.h>
# include <mach/mach.h>

int
clock_gettime(clockid_t id, timespec* ts)
{
  if (id != CLOCK_MONOTONIC
      && id != CLOCK_REALTIME)
  {
    errno = EINVAL;
    return -1;
  }
  if (ts)
  {
    clock_serv_t cclock;
    mach_timespec_t mts;
    // http://lists.apple.com/archives/darwin-development/2002/Nov/msg00092.html
    clock_id_t cid =
      id == CLOCK_REALTIME    ? CALENDAR_CLOCK
      :                         SYSTEM_CLOCK;
    host_get_clock_service(mach_host_self(), cid, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
  }
  return 0;
}

#endif

#if defined WIN32

int
clock_gettime(clockid_t, timespec* t)
{
  if (t)
  {
    t->tv_sec = 0;
    t->tv_nsec = 0;
  }
  return 0;
}

struct tm*
localtime_r(const time_t* clock, tm* res)
{
  if (res)
    *res = *localtime(clock);
  return res;
}

#endif // !defined WIN32
