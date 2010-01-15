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
  {
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

  inline
  Synchronizer::SynchroPoint::SynchroPoint(Synchronizer& src)
    : sync_(src)
  {
    sync_.cond_.lock();
    sync_.waiting_count_++;
    sync_.cond_.wait();
  }

  inline
  Synchronizer::SynchroPoint::~SynchroPoint()
  {
    sync_.signaled_count_--;
    if (!sync_.signaled_count_)
    {
      sync_.done_++;
    }
    sync_.cond_.unlock();
  }
}
