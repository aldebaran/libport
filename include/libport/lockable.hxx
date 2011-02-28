/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_LOCKABLE_HXX
# define LIBPORT_LOCKABLE_HXX

# include <libport/pthread.h>
# include <libport/cassert>
# include <libport/cerrno>
# include <libport/format.hh>

/*--------------------------.
| Implementation: Windows.  |
`--------------------------*/

# if defined WIN32

namespace libport
{
  inline void initLock(Lock& l)
  {
    l = CreateMutex(0, false, 0);
    if (!l)
      // On Windows, the error code is in GetLastError, which is
      // handled gracefully by (our) strerror, which is used by
      // errnoabort.
      errnoabort("CreateMutex");
  }

  inline void lockLock(Lock& l)
  {
    if (WaitForSingleObject(l, INFINITE) == WAIT_FAILED)
      errnoabort("WaitForSingleObject");
  }

  inline void lockUnlock(Lock& l)
  {
    if (!ReleaseMutex(l))
      errnoabort("ReleaseMutex");
  }

  inline void deleteLock(Lock& l)
  {
    if (!CloseHandle(l))
      errnoabort("CloseHandle");
  }

  inline bool lockTryLock(Lock& l)
  {
    DWORD ret = WaitForSingleObject(l, 0);
    if (ret == WAIT_FAILED)
      errnoabort("WaitForSingleObject");
    return ret != WAIT_TIMEOUT;
  }

} // namespace libport


/*---------------------------.
| Implementation: pthreads.  |
`---------------------------*/

# else

namespace libport
{

  inline void initLock(Lock& l)
  {
    pthread_mutexattr_t ma;
    PTHREAD_RUN(pthread_mutexattr_init, &ma);
    /* See
     * http://www.nabble.com/Compiling-on-MacOS-10.4-Tiger-t284385.html
     * for more about this code.  Yes, the second #if is very
     * suspicious, I don't know why it's like this.  */
#  if defined  __APPLE__ || defined __FreeBSD__
    PTHREAD_RUN(pthread_mutexattr_settype, &ma, PTHREAD_MUTEX_RECURSIVE);
#  elif defined PTHREAD_MUTEX_RECURSIVE_NP
    // cygwin
    PTHREAD_RUN(pthread_mutexattr_setkind_np, &ma, PTHREAD_MUTEX_RECURSIVE);
#  else
    // deprecated according to man page and fails to compile
    // pthread_mutexattr_setkind_np(&ma, PTHREAD_MUTEX_RECURSIVE_NP);
    PTHREAD_RUN(pthread_mutexattr_settype, &ma, PTHREAD_MUTEX_RECURSIVE_NP);
#  endif
    PTHREAD_RUN(pthread_mutex_init, &l, &ma);
  }

  inline void lockLock(Lock& l)
  {
    PTHREAD_RUN(pthread_mutex_lock, &l);
  }

  inline void lockUnlock(Lock& l)
  {
    PTHREAD_RUN(pthread_mutex_unlock, &l);
  }

  inline void deleteLock(Lock& l)
  {
    PTHREAD_RUN(pthread_mutex_destroy, &l);
  }

  inline bool lockTryLock(Lock& l)
  {
    switch (int err = pthread_mutex_trylock(&l))
    {
    case 0:
      return true;
    case EBUSY:
      return false;
    default:
      LIBPORT_USE(err);
      errabort(err, "pthread_mutex_trylock");
    }
  }

} // namespace libport

# endif


namespace libport
{

  /*-----------.
  | Lockable.  |
  `-----------*/
  inline
  Lockable::Lockable()
  {
    initLock(lock_);
  }

  inline
  Lockable::~Lockable()
  {
    deleteLock(lock_);
  }

  inline
  void Lockable::lock()
  {
    lockLock(lock_);
  }

  inline
  void Lockable::unlock()
  {
    lockUnlock(lock_);
  }

  inline
  bool Lockable::tryLock()
  {
    return lockTryLock(lock_);
  }


  /*------------.
  | BlockLock.  |
  `------------*/

  inline
  BlockLock::BlockLock(Lockable& l)
    : lockable_(l)
  {
    lockable_.lock();
  }

  inline
  BlockLock::BlockLock(Lockable* l)
    : lockable_(*l)
  {
    //std::cerr <<"lock "<<l<<std::endl;
    lockable_.lock();
  }

  inline
  BlockLock::~BlockLock()
  {
    //std::cerr <<"unlock "<<&l<<std::endl;
    lockable_.unlock();
  }


  inline
  ScopedTryLock::ScopedTryLock(Lockable& l)
    : lockable_(l)
  {
    hasLock_ = lockable_.tryLock();
  }

  inline
  ScopedTryLock::ScopedTryLock(Lockable* l)
    : lockable_(*l)
  {
    hasLock_ = lockable_.tryLock();
  }

  inline
  ScopedTryLock::~ScopedTryLock()
  {
    if (hasLock_)
      lockable_.unlock();
  }

  inline bool
  ScopedTryLock::hasLock() const
  {
    return hasLock_;
  }

  inline
  ScopedTryLock::operator bool() const
  {
    return hasLock();
  }

} // namespace libport

#endif // !LIBPORT_LOCKABLE_HXX
