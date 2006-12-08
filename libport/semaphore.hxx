#ifndef SEMAPHORE_HXX_
# define SEMAPHORE_HXX_

# include "semaphore.hh"

namespace urbi
{

# if defined WIN32 || defined LIBPORT_WIN32
  inline sem_t*
  sem_open (const char* name,
            int oflag,
            unsigned int /*mode_t*/ mode,
            unsigned int value)
  {
    return SEM_FAILED; // Use sem_init instead.
  }

  inline int sem_init(sem_t* sem, int useless, int cnt)
  {
    *sem = CreateSemaphore(0, cnt, 100000, 0);
    return !sem;
  }

  inline int sem_post(sem_t* sem)
  {
    return !ReleaseSemaphore(*sem, 1, 0);
  }

  inline int sem_wait(sem_t* sem)
  {
    return WaitForSingleObject(*sem, INFINITE) != WAIT_FAILED;
  }

  inline int sem_destroy(sem_t* sem)
  {
    return !CloseHandle(*sem);
  }

  inline int sem_getvalue(sem_t* sem, int* v)
  {
    *v = 1; //TODO: implement
    // Maybe look at: http://www.codeguru.com/Cpp/W-P/win32/article.php/c1423
    return 0;
  }
#endif /* !WIN32 */


/* Darwin doesn't implement sem_init/sem_close (although the functions exists
 * and is defined, in fact it just returns ENOSYS). That's why need to use
 * named semaphores (with sem_open) instead.
 * See:
 * http://lists.apple.com/archives/darwin-development/2004/May/msg00077.html
 * http://www.google.com/search?as_q=sem_init&as_sitesearch=apple.com
 *
 * Semaphores created with sem_open must be closed with sem_close.
 * Semaphores created with sem_init must be closed with sem_destroy.
 */
# ifdef __APPLE__
#  include <sstream>
# endif

  inline Semaphore::Semaphore (int cnt)
  {
# ifdef __APPLE__
    std::stringstream s;

    s << "sema_";
    s << counter_++;
    sem = sem_open (s.str ().c_str (), O_CREAT, 0777, cnt);
    assert (!IS_SEM_FAILED (sem));
# else
    assert (sem_init(sem, 0, cnt) == 0);
# endif
  }

  inline Semaphore::~Semaphore ()
  {
# ifdef __APPLE__
    assert (sem_close(sem) == 0);
# else
    assert (sem_destroy(sem) == 0);
# endif
  }


  inline void
  Semaphore::operator++ (int)
  {
    assert (sem_post(sem) == 0);
  }

  inline void
  Semaphore::operator-- (int)
  {
    assert (sem_wait(sem) == 0);
  }

  inline void
  Semaphore::operator++ ()
  {
    assert (sem_post(sem) == 0);
  }

  inline void
  Semaphore::operator-- ()
  {
    assert (sem_wait(sem) == 0);
  }

  inline
  Semaphore::operator int ()
  {
    int t;
    assert (sem_getvalue(sem, &t) == 0);
    return t;
  }

} // namespace urbi

#endif /* !SEMAPHORE_HXX_ */
