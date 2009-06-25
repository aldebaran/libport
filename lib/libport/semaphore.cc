#include <cerrno>
#include <iostream>

#include <libport/assert.hh>
#include <libport/exception.hh>
#include <libport/program-name.hh>
#include <libport/semaphore.hh>
#include <libport/sysexits.hh>
#include <time.h>

namespace libport
{

# if defined WIN32 || defined LIBPORT_WIN32
#  define CLOCK_REALTIME 0
#  define ETIMEDOUT 110

  int
  clock_gettime(int, struct timespec* t)
  {
    if (t)
    {
      t->tv_sec = 0;
      t->tv_nsec = 0;
    }
    return 0;
  }

  sem_t*
  sem_open(const char* /* name */,
           int /* oflag */,
           unsigned int /*mode_t*/ /* mode */,
           unsigned int /* value */)
  {
    return 0; // Use sem_init instead.
  }

  int
  sem_init(sem_t* sem, int, unsigned value)
  {
    *sem = CreateSemaphore(0, value, MAXLONG, 0);
    if (!sem)
      errabort("CreateSemaphore");
    return 0;
  }

  int
  sem_post(sem_t* sem)
  {
    if (!ReleaseSemaphore(*sem, 1, 0))
      errabort("ReleaseSemaphore");
    return 0;
  }

  int
  sem_wait(sem_t* sem)
  {
    if (WaitForSingleObject(*sem, INFINITE) == WAIT_FAILED)
      errabort("WaitForSingleObject");
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
      errabort("WaitForSingleObject");
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
    *v = 1; //TODO: implement
    // Maybe look at: http://www.codeguru.com/Cpp/W-P/win32/article.php/c1423
    return 0;
  }
#endif /* !WIN32 */

} // namespace libport

/* Darwin doesn't implement sem_init/sem_close (although the functions exists
 * and is defined, in fact it just returns ENOSYS). That's why we need to use
 * named semaphores (with sem_open) instead.
 * See:
 * http://lists.apple.com/archives/darwin-development/2004/May/msg00077.html
 * http://www.google.com/search?as_q=sem_init&as_sitesearch=apple.com
 *
 * Semaphores created with sem_open must be closed with sem_close.
 * Semaphores created with sem_init must be closed with sem_destroy.
 */
#ifdef __APPLE__
# include <fstream>
# include <sstream>
# include <sys/stat.h>
# include <sys/time.h>
# include <signal.h>
#endif

namespace libport
{

  Semaphore::Semaphore(unsigned cnt)
  {
# ifdef __APPLE__
    static unsigned int counter = 0;
    int sem_open_errno;

    do
    {
      std::stringstream s;
      errno = 0;
      s << "sema/" << getpid() << "/" << counter++;
      name_ = s.str();
      sem_ = sem_open(name_.c_str(), O_CREAT | O_EXCL, 0777, cnt);
      sem_open_errno = errno;
    } while (sem_ == SEM_FAILED && errno == EEXIST);

    {
      // Save the semaphore name if we can, in order to provide the
      // user with a means to reclaim them.  Don't check for success,
      // this is best effort.
      //
      // If you change this file name, update
      // build-aux/semaphores-clean.sh.
      mkdir("/tmp/urbi-semaphores", 0777);
      std::stringstream f;
      f << "/tmp/urbi-semaphores/" << getpid();
      std::ofstream o(f.str().c_str(), std::ios_base::app);
      o << name_;
      if (sem_ == SEM_FAILED)
        o << ": " << strerror(sem_open_errno);
      o << std::endl;
    }

    if (sem_ == SEM_FAILED)
    {
      std::string error = strerror(sem_open_errno);
      if (sem_open_errno == ENOSPC)
        error += " (i.e., used all the semaphores, run semaphore-clean)";
      // Certainly dying because we don't have enough semaphores
      // available.  Report we should be skipped.
      std::cerr << program_name()
                << ": cannot sem_open(" << name_ << "): "
                << strerror(sem_open_errno)
                << std::endl
                << exit(EX_SKIP);
    }
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

  Semaphore::~Semaphore()
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
  Semaphore::operator++()
  {
    if (sem_post(sem_))
    {
      destroy();
      errabort("sem_post");
    }
  }

  void
  Semaphore::operator--()
  {
    get(0);
  }

  Semaphore& Semaphore::operator-=(unsigned c)
  {
    for (unsigned i = 0; i < c; ++i)
      --*this;
    return *this;
  }

  Semaphore& Semaphore::operator+=(unsigned c)
  {
    for (unsigned i = 0; i < c; ++i)
      ++*this;
    return *this;
  }

# if defined __APPLE__

  // On OSX there seems to be no means to timeout passively, so we
  // launch a thread that will change the following flag when the time
  // is out.
  static bool time_is_out;

  void fun_timeout(int)
  {
    time_is_out = true;
  }

# endif

  bool
  Semaphore::uget(utime_t useconds)
  {
    int err;
    do
    {
      if (useconds == 0)
        err = sem_wait(sem_);
# if defined __APPLE__
      else
      {
	struct itimerval it;
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;

	it.it_value.tv_sec = useconds / 1000000;
	it.it_value.tv_usec = useconds % 1000000;
	time_is_out = false;
	signal(SIGALRM, fun_timeout);
	setitimer(ITIMER_REAL, &it, NULL);
	do
	{
	  err = sem_trywait(sem_);
	} while (!time_is_out && err == -1 && errno == EAGAIN);
	if (time_is_out)
        {
          // Match the interface under Linux.
	  err = -1;
          errno = ETIMEDOUT;
        }
      }
# else
      else
      {
        struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts))
          errabort("clock_gettime");
	ts.tv_sec += useconds / 1000000;
	ts.tv_nsec += (useconds % 1000000) * 1000;
        err = sem_timedwait(sem_, &ts);
      }
# endif
    }
    while (err == -1 && errno == EINTR);

    if (err)
    {
      if (errno == ETIMEDOUT)
        return false;
      destroy();
      errabort("sem_wait");
    }

    return true;
  }

  bool
  Semaphore::get(unsigned seconds)
  {
    return uget(seconds * 1000 * 1000);
  }

  Semaphore::operator int()
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
