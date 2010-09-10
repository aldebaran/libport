/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/semaphore.h>
#include <libport/cassert>
#include <libport/cerrno>

// Provide replacement for semaphore.h functions.  This is different
// from semaphore.cc which implements the libport::Semaphore class.

namespace libport
{

# if defined WIN32 || defined LIBPORT_WIN32

  sem_t*
  sem_open(const char* /* name */,
           int /* oflag */,
           mode_t /* mode */,
           unsigned int /* value */)
  {
    return 0; // Use sem_init instead.
  }

  int
  sem_init(sem_t* sem, int, unsigned value)
  {
    *sem = CreateSemaphore(0, value, MAXLONG, 0);
    if (!sem)
      errnoabort("CreateSemaphore");
    return 0;
  }

  int
  sem_post(sem_t* sem)
  {
    if (!ReleaseSemaphore(*sem, 1, 0))
      errnoabort("ReleaseSemaphore");
    return 0;
  }

  int
  sem_wait(sem_t* sem)
  {
    if (WaitForSingleObject(*sem, INFINITE) == WAIT_FAILED)
      errnoabort("WaitForSingleObject");
    return 0;
  }

  int
  sem_timedwait(sem_t* sem, const struct timespec *abs_timeout)
  {
    switch (WaitForSingleObject(*sem,
                                abs_timeout->tv_sec * 1000
                                + abs_timeout->tv_nsec / 1000000))
    {
    case WAIT_FAILED:
      errnoabort("WaitForSingleObject");
    case WAIT_TIMEOUT:
      errno = ETIMEDOUT;
      return -1;
    default:
      return 0;
    }
  }

  int
  sem_destroy(sem_t* sem)
  {
    return CloseHandle(*sem) ? 0 : -1;
  }

  int
  sem_getvalue(sem_t* /* sem */, int* v)
  {
    //TODO: implement
    // Maybe look at: http://www.codeguru.com/Cpp/W-P/win32/article.php/c1423
    return ENOSYS;
  }
#endif /* !WIN32 */

} // namespace libport
