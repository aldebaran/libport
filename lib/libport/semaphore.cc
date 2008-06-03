#include <libport/assert.hh>
#include <libport/exception.hh>
#include <libport/semaphore.hh>

namespace libport
{

# if defined WIN32 || defined LIBPORT_WIN32
  sem_t*
  sem_open (const char* name,
	    int oflag,
	    unsigned int /*mode_t*/ mode,
	    unsigned int value)
  {
    return SEM_FAILED; // Use sem_init instead.
  }

  int
  sem_init(sem_t* sem, int, int cnt)
  {
    *sem = CreateSemaphore(0, cnt, 100000, 0);
    if (!sem)
      errabort("CreateSemaphore");
    return sem ? 0 : -1;
  }

  int
  sem_post(sem_t* sem)
  {
    unsigned int result = ReleaseSemaphore(*sem, 1, 0);
    if (!result)
      errabort("ReleaseSemaphore");
    return result ? 0 : -1;
  }

  int
  sem_wait(sem_t* sem)
  {
    unsigned int result = WaitForSingleObject(*sem, INFINITE);
    if (result == WAIT_FAILED)
      errabort("WaitForSingleObject");
    return (result == WAIT_FAILED) ? -1 : 0;
  }

  int
  sem_destroy(sem_t* sem)
  {
    return CloseHandle(*sem) ? 0 : -1;
  }

  int
  sem_getvalue(sem_t* sem, int* v)
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

# include <cerrno>

namespace libport
{

# if defined __linux || defined WIN32
/* Linux defines SEM_FAILED as  (sem_t *) 0 */
#  define IS_SEM_FAILED(sem)   ((sem) == SEM_FAILED)
# else
/* MacOSX (and thus BSD?) defines it as -1 */
#  define IS_SEM_FAILED(sem)   ((long) (sem) == (long) SEM_FAILED)
# endif

  Semaphore::Semaphore (int cnt)
  {
# ifdef __APPLE__
    static unsigned int counter = 0;
    std::stringstream s;
    s << "sema/" << getpid() << "/" << counter++;
    name_ = s.str();
    sem_ = sem_open(name_.c_str (), O_CREAT | O_EXCL, 0777, cnt);
    if (IS_SEM_FAILED(sem_))
      errabort("sem_open(" << name_ << ')');
# else
    sem_ = new sem_t;
    if (sem_init(sem_, 0, cnt))
      errabort("sem_init(" << cnt << ')');
# endif
  }

  Semaphore::~Semaphore ()
  {
# ifdef __APPLE__
    if (sem_close(sem_))
      errabort("sem_close");
    if (sem_unlink(name_.c_str()))
      errabort("sem_unlink");
# else
    if (sem_destroy(sem_))
      errabort("sem_destroy");
    delete sem_;
# endif
  }


  void
  Semaphore::operator++ ()
  {
    if (sem_post(sem_))
      errabort("sem_post");
  }

  void
  Semaphore::operator-- ()
  {
    int err;
    do
      {
	err = sem_wait(sem_);
      }
    while (err == -1 && errno == EINTR);
    if (err)
      errabort("sem_wait");
  }

  Semaphore::operator int ()
  {
    int t;
    if (sem_getvalue(sem_, &t))
      errabort("sem_getvalue");
    return t;
  }

} // namespace libport
