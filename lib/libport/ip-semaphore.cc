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

    semun arg;
    arg.val = count;
    semctl(id_, 0, SETVAL, arg);
  }

  static void alter_sem(int id, int val)
  {
    assert(val != 0);
    sembuf sops[1];
    sops[0].sem_num = 0;
    sops[0].sem_op = val;
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
}

#endif
