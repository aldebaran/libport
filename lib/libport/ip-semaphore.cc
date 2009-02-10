#include <libport/config.h>
#ifdef LIBPORT_HAVE_SEMGET

# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/sem.h>

# include <cassert>

# include <libport/ip-semaphore.hh>

namespace libport
{
  IPSemaphore::IPSemaphore(int count)
  {
    // Create a set of 1 semaphore, with permisions rwx------
    id_ = semget(IPC_PRIVATE, 1, 0700);

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
    semctl(id_, 0, SETVAL, arg);
  }

  // Add \a val to semaphore \a id
  static void alter_sem(int id, int val)
  {
    assert(val != 0);
    sembuf sops[1];
    sops[0].sem_num = 0;
    sops[0].sem_op = val;
    // If the process terminates, undo the change. This avoid blocking
    // several processes if a process segv before it releases the
    // semaphore, for instance.
    sops[0].sem_flg = SEM_UNDO;
    semop(id, sops, 1);
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
