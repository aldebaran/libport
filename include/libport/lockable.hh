#ifndef LIBPORT_LOCKABLE_HH
# define LIBPORT_LOCKABLE_HH

# include <libport/config.h>
# include <libport/detect-win32.h>

  /*-------.
  | Lock.  |
  `-------*/

  // FIXME: Hide this.
# if defined WIN32
#  include <libport/windows.hh>
namespace libport
{
  typedef HANDLE Lock;
}
# elif defined LIBPORT_URBI_ENV_AIBO && LIBPORT_URBI_ENV_AIBO
namespace libport
{
  typedef int Lock;
}
# else
# if !defined LIBPORT_HAVE_PTHREAD_H || !LIBPORT_HAVE_PTHREAD_H
#  error "pthreads are required"
# endif
# include <pthread.h>
namespace libport
{
  typedef pthread_mutex_t Lock;
}
# endif


namespace libport
{
  class Condition;
  /*-----------.
  | Lockable.  |
  `-----------*/

  class Lockable
  {
  public:
    Lockable();
    ~Lockable();
    void lock();
    void unlock();
    bool tryLock();
  private:
    Lock lock_;
    friend class Condition;
  };


  /*------------.
  | BlockLock.  |
  `------------*/

  class BlockLock
  {
  public:
    BlockLock(Lockable& l);
    BlockLock(Lockable* l);
    ~BlockLock();

  private:
    Lockable& lockable_;
  };

} // namespace libport

# ifdef LOCKED
#  undef LOCKED
# endif

# define LOCKED(Lock, Cmd)			\
  do {						\
    Lock.lock();				\
    Cmd;					\
    Lock.unlock();				\
  } while (0)

# include <libport/lockable.hxx>

#endif // !LIBPORT_LOCKABLE_HH
