#ifndef LIBPORT_SEMAPHORE_HXX
# define LIBPORT_SEMAPHORE_HXX

# include <cerrno>

# include <libport/assert.hh>
# include <libport/semaphore.hh>

namespace libport
{

  inline void
  Semaphore::operator++(int)
  {
    Semaphore::operator++();
  }

  inline void
  Semaphore::operator--(int)
  {
    Semaphore::operator--();
  }

  inline
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

} // namespace libport

#endif // !LIBPORT_SEMAPHORE_HXX
