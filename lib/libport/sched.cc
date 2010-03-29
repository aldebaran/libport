/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>

#include <algorithm>
#include <libport/cassert>
#include <libport/cstring>
#include <iostream>

#include <libport/unistd.h>

#if defined LIBPORT_HAVE_SCHED_H
# include <sched.h>
#endif
#if defined LIBPORT_HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif
#if defined LIBPORT_HAVE_SYS_MMAN_H
# include <sys/mman.h>
#endif

#include <libport/utime.hh>
#include <libport/sched.hh>
#include <libport/windows.hh>

namespace libport
{
  int
  sched_estimate_granularity()
  {
    //evaluate scheduler granularity
    utime_t mind = 1000000000;
    for (int i=0; i<10;i++)
    {
      utime_t start = libport::utime();
      for (int j=0; j<10;j++)
	usleep(0);
      mind = std::min(mind, libport::utime() - start);
    }
    mind /= 10;

    //we often get a slightly underevaluated value so compensate
    if ((mind/1000 + 1) * 1000 - mind < 100)
      mind = (mind/1000 + 1) * 1000;
    return mind;
  }

  namespace
  {
    // write to a big buffer on stack to ensure allocation
    static void
    lockStack()
    {
      // see man page for mlockall
      char big[16384];
      memset(big, 0, 16384);
    }
  }

  void sched_set_high_priority ()
  {
#if defined WIN32
    HANDLE hProcess = GetCurrentProcess();
    int stat = SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
    if (!stat)
      std::cerr << "SetPriorityClass failed: error code "<< GetLastError()
		<< std::endl;

#elif defined LIBPORT_HAVE_SCHED_SETSCHEDULER
    // http://www.opengroup.org/susv3xsh/sched_setscheduler.html
    struct sched_param sp;
    sp.sched_priority = 99;
    ERRNO_RUN(sched_setscheduler, 0, SCHED_FIFO, &sp);
#elif defined LIBPORT_HAVE_SETPRIORITY
    ERRNO_RUN(setpriority, PRIO_PROCESS, 0, -20);
#endif

#if defined LIBPORT_HAVE_MLOCKALL
    ERRNO_RUN(mlockall, MCL_CURRENT | MCL_FUTURE);
#endif
  lockStack();
  }
}
