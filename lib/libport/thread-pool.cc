/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/utime.hh>
#include <libport/thread-pool.hh>
#include <libport/thread.hh>

// Not using GD for debug, we need a 0-cost when disabled.
#define debug(a)
//#define debug(a) std::cerr << libport::utime() << ' ' << a << std::endl

namespace libport
{
  ThreadPool::ThreadPool(size_t maxThreads)
    : maxThreads_(maxThreads)
  {
  }

  size_t
  ThreadPool::queueSize()
  {
    libport::BlockLock bl(lock_);
    return queue_.size() + nLockedTasks_;
  }

  void
  ThreadPool::threadLoop(rThread thread)
  {
    // will wait on our sem if true
    debug("new thread start");
    bool waiting = true;
    while (true)
    {
      debug(" thread waiting? " << waiting);
      if (waiting)
      {
        thread->sem--; // our task was set by another thread
        debug("thread done waiting");
        waiting = false;
      }
      else if (thread->taskLock)
      {
        debug("handling taskLock task");
        libport::BlockLock bl(lock_);
        if (thread->taskLock->waitingTasks.empty())
        { // No more tasks, deregister.
          debug("deregistering taskLock");
          thread->taskLock->handler = 0;
          thread->taskLock->registered = false;
          thread->taskLock = 0;
          continue;
        }
        else
        {
          debug("popping taskLock task");
          thread->currentTask = thread->taskLock->waitingTasks.front();
          thread->taskLock->waitingTasks.pop_front();
          nLockedTasks_--;
        }
      }
      else
      {
        debug("thread fetching task to main queue");
        // Go fetch ourselve a task
        libport::BlockLock bl(lock_);
        if (threads_.size() > maxThreads_ && maxThreads_)
        {
          debug("thread diing");
          // maxThreads_ was decreased, die
          threads_.remove(thread);
          return;
        }
        if (queue_.empty())
        {
          // Nothing to do, register as idle.
          debug("queue empty, will wait");
          idleThreads_.push_back(thread);
          waiting = true;
          continue;
        }
        thread->currentTask = queue_.front();
        queue_.pop_front();
        // If current task was associated to a taskLock, handle it from now.
        thread->taskLock = thread->currentTask->taskLock;
      }
      thread->currentTask->state_ = TaskHandle::RUNNING;
      try {
        thread->currentTask->taskFunc();
        thread->currentTask->state_ = TaskHandle::FINISHED;
      }
      catch(...)
      {
        thread->currentTask->state_ = TaskHandle::EXCEPTION;
      }
      if (thread->currentTask->taskLock)
        thread->taskLock = thread->currentTask->taskLock;
      thread->currentTask = 0;
    }
  }

  ThreadPool::rTaskHandle
  ThreadPool::queueTask(TaskFunc func, rTaskLock lock)
  {
    rTaskHandle res(new TaskHandle);
    res->taskFunc = func;
    res->taskLock = lock;
    /* Three cases:
       - Task depends on another running task: add to this dependency queue.
       - We have IDLE threads: wake one up
       - Thread count is below limit: spawn a thread.
       - Else: queue
    */
    libport::BlockLock bl(lock_);
    if (lock && lock->registered)
    {
      if (lock->maxSize && lock->waitingTasks.size() >= lock->maxSize -1)
      {
        debug("queuetask: dropping task");
        res->state_ = TaskHandle::DROPPED;
        return res;
      }
      debug("queuetask: registered taskLock");
      lock->waitingTasks.push_back(res);
      nLockedTasks_++;
    }
    else if (!idleThreads_.empty())
    {
      debug("queuetask to idle thread");
      rThread t = idleThreads_.back();
      idleThreads_.pop_back();
      t->currentTask = res;
      t->sem++;
    }
    else if (threads_.size() < maxThreads_ || !maxThreads_)
    {
      debug("queuetask to new thread");
      rThread rt(new Thread);
      threads_.push_back(rt);
      rt->handle = libport::startThread(boost::bind(&ThreadPool::threadLoop,
                                                    this, rt));
      rt->currentTask = res;
      rt->sem++;
    }
    else
      queue_.push_back(res);
    if (lock)
      lock->registered = true;
    res->state_ = TaskHandle::QUEUED;
    return res;
  }

  ThreadPool::Thread::~Thread()
  {
  }

  ThreadPool::TaskLock::TaskLock()
    : registered(false)
    , maxSize(0)
  {
  }

  ThreadPool::TaskLock::TaskLock(unsigned int maxSize)
    : registered(false)
    , maxSize(maxSize)
  {
  }

  ThreadPool::TaskHandle::State
  ThreadPool::TaskHandle::getState() const
  {
    return state_;
  }

  void
  ThreadPool::resize(size_t maxThreads)
  {
    maxThreads_ = maxThreads;
  }
};
