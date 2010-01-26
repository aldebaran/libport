/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

namespace libport
{

  inline
  Synchronizer::Synchronizer()
    : waiting_count_(0)
    , signaled_count_(0)
  , master_thread_(pthread_self())
  , owner_thread_(master_thread_)
  {
  }

  inline pthread_t
  Synchronizer::getMasterThread()
  {
    return master_thread_;
  }

  inline void
  Synchronizer::setMasterThread(pthread_t id)
  {
    master_thread_ = id;
  }

  inline void
  Synchronizer::setOnLock(boost::function0<void> func)
  {
    on_lock_ = func;
  }

  inline
  void
  Synchronizer::check()
  {
    if (waiting_count_)
    {
      {
        libport::BlockLock bl(cond_);
        signaled_count_ = waiting_count_;
        waiting_count_ = 0;
        cond_.broadcast();
      } // Release cond_ lock before waiting on semaphore
      done_--;
    }
  }

  inline void
  Synchronizer::lock()
  {
    cond_.lock();
    waiting_count_++;
    if (on_lock_)
      on_lock_();
    cond_.wait();
    owner_thread_ = pthread_self(); // must be after locking.
  }

  inline void
  Synchronizer::unlock()
  {
    signaled_count_--;
    if (!signaled_count_)
    {
      done_++;
    }
    owner_thread_ = master_thread_; // must be before unlocking
    cond_.unlock();
  }

  inline
  Synchronizer::SynchroPoint::SynchroPoint(Synchronizer& src, bool check)
    : sync_(src)
  , noop_(check &&
          (pthread_self() == src.getMasterThread()
           || pthread_self() == src.owner_thread_))
  {
    if (!noop_)
    {
      sync_.lock();
    }
  }

  inline
  Synchronizer::SynchroPoint::~SynchroPoint()
  {
    if (!noop_)
    {
      sync_.unlock();
    }
  }

  inline bool Synchronizer::SynchroPoint::isNoop() const
  {
    return noop_;
  }
}
