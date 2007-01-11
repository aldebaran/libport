#ifndef SEMAPHORE_HXX_
# define SEMAPHORE_HXX_

# include "semaphore.hh"

namespace libport
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

} // namespace libport

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

namespace libport
{

  inline Semaphore::Semaphore (int cnt)
  {
# ifdef __APPLE__
    static unsigned int counter = 0;
    std::stringstream s;
    s << "sema_" << counter++;
    sem_ = sem_open (s.str ().c_str (), O_CREAT, 0777, cnt);
    assert (!IS_SEM_FAILED (sem_));
# else
    sem_ = new sem_t;
    int err = sem_init(sem_, 0, cnt);
    assert (!err);
# endif
  }

  inline Semaphore::~Semaphore ()
  {
# ifdef __APPLE__
    int err = sem_close(sem_);
# else
    int err = sem_destroy(sem_);
    delete sem_;
# endif
    assert (err);
  }


  inline void
  Semaphore::operator++ (int)
  {
    int err = sem_post(sem_);
    assert (!err);
  }

  inline void
  Semaphore::operator-- (int)
  {
    int err = sem_wait(sem_);
    assert (!err);
  }

  inline void
  Semaphore::operator++ ()
  {
    int err = sem_post(sem_);
    assert (!err);
  }

  inline void
  Semaphore::operator-- ()
  {
    int err = sem_wait(sem_);
    assert (!err);
  }

  inline
  Semaphore::operator int ()
  {
    int t;
    int err = sem_getvalue(sem_, &t);
    assert (!err);
    return t;
  }

} // namespace libport

#endif /* !SEMAPHORE_HXX_ */
