#include "libport/config.h"

#include <algorithm>
#include <cstring>

#if defined LIBPORT_HAVE_SCHED_H
# include "sched.h"
#endif
#if defined LIBPORT_HAVE_SYS_RESOURCE_H
# include "sys/resource.h"
#endif
#if defined LIBPORT_HAVE_SYS_MMAN_H
# include "sys/mman.h"
#endif

#include "libport/utime.hh"
#include "libport/sched.hh"
#include "libport/windows.hh"

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
    HANDLE hProcess;
    int stat;
    hProcess = GetCurrentProcess();
    stat = SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
    if (!stat)
      std::cerr << "SetPriorityClass failed: error code "<< GetLastError()
		<< std::endl;

#elif defined LIBPORT_HAVE_SCHED_SETSCHULER
    // http://www.opengroup.org/susv3xsh/sched_setscheduler.html
    struct sched_param sp;

    sp.sched_priority = 99;
    int ret = sched_setscheduler(0, SCHED_FIFO, &sp);
    if (ret)
      perror("sched_setscheduler failed");
#elif defined LIBPORT_HAVE_SETPRIORITY
    int ret = setpriority(PRIO_PROCESS, 0, -20);
    if (ret)
       perror("setpriority failed");
#endif

#if defined LIBPORT_HAVE_MLOCKALL
    int r = mlockall(MCL_CURRENT | MCL_FUTURE);
    if (r)
      perror("mlockall failed");
#endif
  lockStack();
  }
}
