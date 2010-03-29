/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef IP_SEMAPHORE_HH
# define IP_SEMAPHORE_HH

# include <libport/config.h>

# ifdef LIBPORT_HAVE_SEMGET
#  define LIBPORT_HAVE_IP_SEMAPHORE

#  include <libport/sys/types.h>
#  include <libport/unistd.h>

namespace libport
{
  // An inter-process semaphore
  // FIXME: works only under Unix
  //        (actually, only when semget/semctl/semop are available)
  class IPSemaphore
  {
  public:
    IPSemaphore(int count = 0);
    ~IPSemaphore();
    void operator++();
    void operator--();

    class Lock
    {
    public:
      Lock(IPSemaphore& sem);
      ~Lock();
    private:
      IPSemaphore& sem_;
    };

  private:
    int id_;
    pid_t owner_pid_;
  };
}

# endif
#endif
