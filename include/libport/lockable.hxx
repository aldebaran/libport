#ifndef LIBPORT_LOCKABLE_HXX
# define LIBPORT_LOCKABLE_HXX

#include <libport/assert.hh>

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
      errabort("CreateMutex");
  }

  inline void lockLock(Lock& l)
  {
    if (WaitForSingleObject(l, INFINITE) ==  WAIT_FAILED)
      errabort("WaitForSingleObject");
  }

  inline void lockUnlock(Lock& l)
  {
    if (!ReleaseMutex(l))
      errabort("ReleaseMutex");
  }

  inline void deleteLock(Lock& l)
  {
    CloseHandle(l);
  }

  inline bool lockTryLock(Lock& l)
  {
    DWORD ret = WaitForSingleObject(l, 0);
    if (ret == WAIT_FAILED)
      errabort("WaitForSingleObject");
    return ret != WAIT_TIMEOUT;
  }
} // namespace libport


/*-----------------------.
| Implementation: AIBO.  |
`-----------------------*/

# elif defined LIBPORT_URBI_ENV_AIBO && LIBPORT_URBI_ENV_AIBO

namespace libport
{

  inline void initLock(Lock&)
  {
  }

  inline void lockLock(Lock&)
  {
  }

  inline void lockUnlock(Lock&)
  {
  }

  inline void deleteLock(Lock&)
  {
  }

  inline bool lockTryLock(Lock&)
  {
    return true;
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
    pthread_mutexattr_init(&ma);
    /* See
     * http://www.nabble.com/Compiling-on-MacOS-10.4-Tiger-t284385.html
     * for more about this code.  Yes, the second #if is very
     * suspicious, I don't know why it's like this.  */
#  if defined  __APPLE__ || defined __FreeBSD__
    pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_RECURSIVE);
#  elif defined PTHREAD_MUTEX_RECURSIVE_NP
    // cygwin
    pthread_mutexattr_setkind_np(&ma, PTHREAD_MUTEX_RECURSIVE);
#  else
    // deprecated according to man page and fails to compile
    // pthread_mutexattr_setkind_np(&ma, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_RECURSIVE_NP);
#  endif
    pthread_mutex_init(&l, &ma);
  }

  inline void lockLock(Lock& l)
  {
    pthread_mutex_lock(&l);
  }

  inline void lockUnlock(Lock& l)
  {
    pthread_mutex_unlock(&l);
  }

  inline void deleteLock(Lock& l)
  {
    pthread_mutex_destroy(&l);
  }

  inline bool lockTryLock(Lock& l)
  {
    return !pthread_mutex_trylock(&l);
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
    libport::initLock(lock_);
  }

  inline
  Lockable::~Lockable()
  {
    libport::deleteLock(lock_);
  }

  inline
  void Lockable::lock()
  {
    libport::lockLock(lock_);
  }

  inline
  void Lockable::unlock()
  {
    libport::lockUnlock(lock_);
  }

  inline
  bool Lockable::tryLock()
  {
    return libport::lockTryLock(lock_);
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

} // namespace libport

#endif // !LIBPORT_LOCKABLE_HXX
