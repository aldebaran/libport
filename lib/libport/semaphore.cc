#include <libport/assert.hh>
#include <libport/exception.hh>
#include <libport/semaphore.hh>

namespace libport
{

# if defined WIN32 || defined LIBPORT_WIN32
  sem_t*
  sem_open (const char* /* name */,
	    int /* oflag */,
	    unsigned int /*mode_t*/ /* mode */,
	    unsigned int /* value */)
  {
    return 0; // Use sem_init instead.
  }

  int
  sem_init(sem_t* sem, int, int cnt)
  {
    *sem = CreateSemaphore(0, cnt, MAXLONG, 0);
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
  sem_getvalue(sem_t* /* sem */, int* v)
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
#  include <fstream>
#  include <sstream>
# endif

# include <cerrno>

namespace libport
{

  Semaphore::Semaphore (int cnt)
  {
# ifdef __APPLE__
    static unsigned int counter = 0;
    std::stringstream s;
    s << "sema/" << getpid() << "/" << counter++;
    name_ = s.str();

    sem_ = sem_open(name_.c_str(), O_CREAT | O_EXCL, 0777, cnt);

    {
      // Save the semaphore name if we can, in order to provide the
      // user with a means to reclaim them.  Don't check for success,
      // this is best effort.
      //
      // If you change this file name, update
      // build-aux/semaphores-clean.sh.
      std::ofstream o("/tmp/urbi-semaphores.log", std::ios_base::app);
      o << name_;
      if (sem_ == SEM_FAILED)
        o << ": " << strerror(errno);
      o << std::endl;
    }

    if (sem_ == SEM_FAILED)
      errabort("sem_open(" << name_ << ')');
# else
    sem_ = new sem_t;
    if (sem_init(sem_, 0, cnt))
    {
      destroy();
      errabort("sem_init(" << cnt << ')');
    }
# endif
    ++instances_;
  }

  Semaphore::~Semaphore ()
  {
    destroy();
    --instances_;
  }

  size_t Semaphore::instances_ = 0;

  size_t
  Semaphore::instances()
  {
    return instances_;
  }

  void
  Semaphore::destroy()
  {
# ifdef __APPLE__
    // We *really* need to unlink the semaphores when we are done,
    // otherwise they leak, and at some point, ENOSPCE is the only
    // answer from the system.  I could not find a means to remove
    // them from the shell (ipcs shows nothing), so I had to reboot my
    // machine.
    //
    // So really, try to call sem_unlink in all the cases, including
    // aborts here.
    //
    // http://lists.apple.com/archives/darwin-dev/2005/Jun/msg00078.html
    int c = sem_close(sem_);
    int close_errno = errno;
    int u = sem_unlink(name_.c_str());
    int unlink_errno = errno;
    if (c)
    {
      errno = close_errno;
      errabort("sem_close");
    }
    if (u)
    {
      errno = unlink_errno;
      errabort("sem_unlink");
    }
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
    {
      destroy();
      errabort("sem_post");
    }
  }

  void
  Semaphore::operator-- ()
  {
    get(0);
  }

  bool
  Semaphore::get(const int timeout)
  {
    int err;
    do
    {
      if (timeout == 0)
        err = sem_wait(sem_);
# if defined __linux
      else
      {
        struct timespec ts;
        ts.tv_sec = time(NULL) + timeout;
        ts.tv_nsec = 0;
        err = sem_timedwait(sem_, &ts);
      }
# else
      else
        pabort("Using timeout is implemented only on linux.");
# endif
    }
    while (err == -1 && errno == EINTR);

# if defined __linux
    if (err && errno == ETIMEDOUT)
      return false;
# endif

    if (err)
    {
      destroy();
      errabort("sem_wait");
    }
    return true;
  }

  Semaphore::operator int ()
  {
    int res;
    if (sem_getvalue(sem_, &res))
    {
      destroy();
      errabort("sem_getvalue");
    }
    return res;
  }

} // namespace libport
