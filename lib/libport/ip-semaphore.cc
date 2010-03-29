/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>
#ifdef LIBPORT_HAVE_SEMGET

# include <libport/cerrno>
# include <libport/cstdio>
# include <libport/cstdlib>
# include <sys/ipc.h>
# include <sys/sem.h>
# include <libport/sys/types.h>

# include <libport/cassert>
# include <libport/ip-semaphore.hh>

// These functions return -1 on error, and set errno.
#  define XRUN(Function, Args)                  \
  do {                                          \
    if ((Function Args) == -1)                  \
      errnoabort(#Function);                    \
  } while (false)

namespace libport
{
  IPSemaphore::IPSemaphore(int count)
    : id_(-1)
    , owner_pid_(getpid())
  {
    // Create a set of 1 semaphore, with permisions rwx------
    id_ = semget(IPC_PRIVATE, 1, 0700);
    if (id_ == -1)
      errnoabort("Unable to create an inter-process semaphore");

    // The man specifies that this union must be defined by the
    // caller. C's ways are past finding out.
    union semun {
      int              val;    /* Value for SETVAL */
      struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
      unsigned short  *array;  /* Array for GETALL, SETALL */
      struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                  (Linux-specific) */
    };

    // Initialize its value to count
    semun arg;
    arg.val = count;
    XRUN(semctl, (id_, 0, SETVAL, arg));
  }

  IPSemaphore::~IPSemaphore()
  {
    if (getpid() == owner_pid_)
      XRUN(semctl, (id_, 0, IPC_RMID));
  }

  // Add \a val to semaphore \a id
  static void alter_sem(int id, int val)
  {
    aver(val);
    sembuf sops[1];
    sops[0].sem_num = 0;
    sops[0].sem_op = val;
    // If the process terminates, undo the change. This avoid blocking
    // several processes if a process segv before it releases the
    // semaphore, for instance.
    sops[0].sem_flg = SEM_UNDO;
    XRUN(semop, (id, sops, 1));
  }

  void
  IPSemaphore::operator++()
  {
    alter_sem(id_, 1);
  }

  void
  IPSemaphore::operator--()
  {
    alter_sem(id_, -1);
  }

  IPSemaphore::Lock::Lock(IPSemaphore& sem)
    : sem_(sem)
  {
    --sem_;
  }

  IPSemaphore::Lock::~Lock()
  {
    ++sem_;
  }
}

#endif
