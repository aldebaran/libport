/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cerrno>
#include <iostream>

#include <libport/cassert>
#include <libport/program-name.hh>
#include <libport/semaphore.hh>
#include <libport/sysexits.hh>
#include <libport/ctime>

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
# include <libport/csignal>
# include <libport/format.hh>
# include <libport/sys/stat.h>
# include <libport/sys/time.h>
# include <sstream>
#endif

#ifdef __XENO__
# ifndef LIBPORT_HAVE_MLOCKALL
#  error "LIBPORT_HAVE_MLOCKALL is required when building for xenomai."
# endif

#include <sys/mman.h>
namespace {
  class MLInit
  {
    public:
    MLInit()
    {
      std::cerr <<"locking all memory" << std::endl;
      if (!mlockall(MCL_CURRENT | MCL_FUTURE))
        perror("mlockall");
    }
  };

  MLInit mlinit;
}

#endif

namespace libport
{

  Semaphore::Semaphore(unsigned value)
  {
# ifdef __APPLE__
    static unsigned int counter = 0;
    int sem_open_errno;

    do
    {
      name_ = format("sema/%s/%s", getpid(), counter++);
      errno = 0;
      sem_ = sem_open(name_.c_str(), O_CREAT | O_EXCL, 0777, value);
      sem_open_errno = errno;
    } while (sem_ == SEM_FAILED && errno == EEXIST);

    {
      // Save the semaphore name if we can, in order to provide the
      // user with a means to reclaim them.  Don't check for success,
      // this is best effort.
      //
      // If you change this file name, update
      // build-aux/semaphores-clean.sh.
      std::string dir = "/tmp/urbi-semaphores." + xgetenv("USER");
      mkdir(dir.c_str(), 0777);
      std::string file = format("%s/%s", dir, getpid());
      std::ofstream o(file.c_str(), std::ios_base::app);
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
    if (sem_init(sem_, 0, value))
    {
      destroy();
      errnoabort("sem_init(" << value << ')');
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
      LIBPORT_USE(close_errno);
      errabort(close_errno, "sem_close");
    }
    if (u)
    {
      LIBPORT_USE(unlink_errno);
      errabort(unlink_errno, "sem_unlink");
    }
# else
    if (sem_destroy(sem_))
      errnoabort("sem_destroy");
    delete sem_;
# endif
  }


  void
  Semaphore::operator++()
  {
    if (sem_post(sem_))
    {
      destroy();
      errnoabort("sem_post");
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
    while (true)
    {
      int err;
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
          errnoabort("clock_gettime");
        ts.tv_sec += useconds / (1000 * 1000);
        ts.tv_nsec += (useconds % (1000 * 1000)) * 1000;
        // Respect the constraints.
        ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
        ts.tv_nsec %= 1000 * 1000 * 1000;
        err = sem_timedwait(sem_, &ts);
      }
# endif

      switch (err ? errno : 0)
      {
      case 0:
        return true;
      case ETIMEDOUT:
        // Failure.
        return false;
      case EINTR:
        // Try again.
        continue;
      default:
        destroy();
        errnoabort("sem_wait");
      }
    }
  }

  int
  Semaphore::value() const
  {
    int res = -1;
#ifdef __APPLE__
    errno = ENOSYS;
#else
    if (sem_getvalue(sem_, &res))
    {
      const_cast<Semaphore*>(this)->destroy();
      errnoabort("sem_getvalue");
    }
#endif
    return res;
  }

  bool
  Semaphore::get(unsigned seconds)
  {
    return uget(seconds * 1000 * 1000);
  }

} // namespace libport
