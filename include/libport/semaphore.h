#ifndef LIBPORT_SEMAPHORE_H
# define LIBPORT_SEMAPHORE_H

# include <libport/detect-win32.h>
# include <libport/export.hh>
# include <libport/time.h>

// Get sem_t.
# if defined WIN32
#  include <libport/windows.hh>

/*------------------------.
| FIXME: Why in libport?  |
`------------------------*/

namespace libport
{
  typedef HANDLE sem_t;

  typedef unsigned int mode_t;

  LIBPORT_API
  sem_t* sem_open(const char* name, int oflag, mode_t mode, unsigned int value);

  LIBPORT_API
  int sem_init(sem_t* sem, int, unsigned value);

  LIBPORT_API
  int sem_post(sem_t* sem);

  LIBPORT_API
  int sem_wait(sem_t* sem);

  LIBPORT_API
  int sem_timedwait(sem_t* sem, const struct timespec *abs_timeout);

  LIBPORT_API
  int sem_destroy(sem_t* sem);

  LIBPORT_API
  int sem_getvalue(sem_t* /* sem */, int* v);

}
# else /* !WIN32 */
#  include <semaphore.h>
# endif

#endif // ! LIBPORT_SEMAPHORE_H
