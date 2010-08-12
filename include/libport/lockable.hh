/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
# elif LIBPORT_HAVE_PTHREAD_H
# include <libport/pthread.h>
namespace libport
{
  typedef pthread_mutex_t Lock;
}
# else
#  error "pthreads are required"
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

  /*----------------.
  | ScopedTryLock.  |
  `----------------*/

  class ScopedTryLock
  {
  public:
    ScopedTryLock(Lockable& l);
    ScopedTryLock(Lockable* l);
    ~ScopedTryLock();
    operator bool () const;
    bool hasLock() const;
  private:
    Lockable& lockable_;
    bool hasLock_;
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
