/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef WIN32
# include <libport/sys/time.h>
# include <libport/cerrno>
#endif

#include <libport/cassert>
#include <libport/exception.hh>

namespace libport
{
#ifndef WIN32

  inline
  Condition::Condition()
  {
    PTHREAD_RUN(pthread_cond_init, &cond_, 0);
  }

  inline void
  Condition::wait()
  {
    while (true)
      switch (int err = pthread_cond_wait(&cond_, &lock_))
      {
      case 0:
        return;
      case EINTR:
        continue;
      default:
        LIBPORT_USE(err);
        errabort(err, "pthread_cond_wait");
      }
  }

  inline bool
  Condition::tryWait(useconds_t duration)
  {
    struct timeval now;
    gettimeofday(&now, 0);

    struct timespec timeout;
    timeout.tv_sec = now.tv_sec + duration / 1000000;
    timeout.tv_nsec = (now.tv_usec + (duration % 1000000)) * 1000;
    timeout.tv_sec += timeout.tv_nsec / 1000000000;
    timeout.tv_nsec %= 1000000000;

    while (true)
      switch (int err = pthread_cond_timedwait(&cond_, &lock_, &timeout))
      {
      case 0:
        return true;
      case ETIMEDOUT:
        return false;
      case EINTR:
        continue;
      default:
        LIBPORT_USE(err);
        errabort(err, "pthread_cond_timedwait");
      }
  }

  inline void
  Condition::signal()
  {
    PTHREAD_RUN(pthread_cond_signal, &cond_);
  }

  inline void
  Condition::broadcast()
  {
    PTHREAD_RUN(pthread_cond_broadcast, &cond_);
  }

#else  // WIN32.

  inline
  Condition::Condition()
    : readers_count_(0)
  {

  }

  inline void
  Condition::signal()
  {
    // The user may or may not have the lock, so take it.
    BlockLock bl(this);
    // Just release one of the waiters it its there.
    if (readers_count_)
      sem_++;
  }

  inline void
  Condition::broadcast()
  {
    // Since wait will try to ackquire the lock, no new wait can insert itself
    // after this point, and be woken up by our broadcast
    BlockLock bl(this);
    for (size_t i = 0; i < readers_count_; ++i)
      sem_++;
  }

  inline void
  Condition::wait()
  {
    // We are locked at this point.
    ++readers_count_;
    if (SignalObjectAndWait(lock_, *sem_.sem_, INFINITE, false) == WAIT_FAILED)
      errnoabort("SignalObjectAndWait");
    // Relock is required by API, but also ensures user does not signal or wait
    // again before broadcast has finished.
    lock();
    --readers_count_;
  }

  inline bool
  Condition::tryWait(useconds_t duration)
  {
    // We are locked at this point.
    ++readers_count_;
    DWORD res = SignalObjectAndWait(lock_, *sem_.sem_, duration / 1000,
                                    false);
    if (res == WAIT_FAILED)
      errnoabort("SignalObjectAndWait");
    // Relock is required by API, but also ensures user does not signal or wait
    // again before broadcast has finished.
    lock();
    --readers_count_;
    return res != WAIT_TIMEOUT;
  }

#endif
}
