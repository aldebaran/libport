#ifndef LIBPORT_LOCKABLE_HXX
# define LIBPORT_LOCKABLE_HXX

# include <libport/assert.hh>
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


/*---------------------------.
| Implementation: pthreads.  |
`---------------------------*/

# else

namespace libport
{

  inline void initLock(Lock& l)
  {
    pthread_mutexattr_t ma;
    if (pthread_mutexattr_init(&ma))
      errabort("pthread_mutexattr_init");
    /* See
     * http://www.nabble.com/Compiling-on-MacOS-10.4-Tiger-t284385.html
     * for more about this code.  Yes, the second #if is very
     * suspicious, I don't know why it's like this.  */
#  if defined  __APPLE__ || defined __FreeBSD__
    if (pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_RECURSIVE))
      errabort("pthread_mutexattr_settype");
#  elif defined PTHREAD_MUTEX_RECURSIVE_NP
    // cygwin
    if (pthread_mutexattr_setkind_np(&ma, PTHREAD_MUTEX_RECURSIVE))
      errabort("pthread_mutexattr_setkind_np");
#  else
    // deprecated according to man page and fails to compile
    // pthread_mutexattr_setkind_np(&ma, PTHREAD_MUTEX_RECURSIVE_NP);
    if (pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_RECURSIVE_NP))
      errabort("pthread_mutexattr_settype");
#  endif
    if (pthread_mutex_init(&l, &ma))
      errabort("pthread_mutex_init");
  }

  inline void lockLock(Lock& l)
  {
    if (pthread_mutex_lock(&l))
      errabort("pthread_mutex_lock");
  }

  inline void lockUnlock(Lock& l)
  {
    if (pthread_mutex_unlock(&l))
      errabort("pthread_mutex_unlock");
  }

  inline void deleteLock(Lock& l)
  {
    if (pthread_mutex_destroy(&l))
      errabort("pthread_mutex_destroy");
  }

  inline bool lockTryLock(Lock& l)
  {
    if (pthread_mutex_trylock(&l))
    {
      if (errno == EBUSY)
        return false;
      else
        errabort("pthread_mutex_trylock");
    }
    else
      return true;
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

} // namespace libport

#endif // !LIBPORT_LOCKABLE_HXX
