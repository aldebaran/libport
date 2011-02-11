/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_THREAD_POOL_HH
# define LIBPORT_THREAD_POOL_HH

# include <list>

# include <boost/function.hpp>

# include <libport/export.hh>
# include <libport/intrusive-ptr.hh>
# include <libport/lockable.hh>
# include <libport/ref-counted.hh>
# include <libport/semaphore.hh>

namespace libport
{
  /** Simple ThreadPool implementation.
   * This is a shared-queue ThreadPool implementation with support for
   * 'locks' that prevents tasks sharing the same lock from executing in
   * parallel.
   */
  class LIBPORT_API ThreadPool
  {
  public:
    typedef boost::function0<void> TaskFunc;
    class TaskLock;
    class TaskHandle;
  private:
    class Thread;
    typedef libport::intrusive_ptr<Thread> rThread;
  public:
    typedef libport::intrusive_ptr<TaskHandle> rTaskHandle;
    typedef libport::intrusive_ptr<TaskLock> rTaskLock;

    /// Class representing a task.
    class LIBPORT_API TaskHandle: public ThreadSafeRefCounted
    {
    public:
      TaskFunc taskFunc;
      rTaskLock taskLock;
      // State is only modified by handler thread: not locked.
      enum State
      {
        QUEUED,
        RUNNING,
        FINISHED,
        EXCEPTION,
        DROPPED
      };
      State getState() const;
    private:
      State state_;
      friend class ThreadPool;
    };

    /// Class storing a set of tasks that must not run in parallel.
    class LIBPORT_API TaskLock: public ThreadSafeRefCounted
    {
    public:
      TaskLock();
      /** Drop tasks so that the queue never gets bigger than maxSize-1 (0=inf)
      * So maxSize=1 means that everything is dropped while another task is
      * running.
      */
      TaskLock(unsigned int maxSize);
    private:
      std::list<rTaskHandle> waitingTasks;
      bool registered;
      /// Thread handling this task set.
      rThread handler;
      friend class ThreadPool;
      unsigned int maxSize;
    };

    /// Create a new thread pool that can grow up to \b maxThreads threads.
    ThreadPool(size_t maxThreads = 0);

    /// Set maximum number of threads in pool
    void resize(size_t maxThreads);

    /** Queue a new task.
    *  @param func the function to execute when the task is scheduled.
    *  @param lock inter-task lock: no two tasks with the same lock will be
    *    allowed to run at the same time.
    */
    rTaskHandle queueTask(TaskFunc func,
                          rTaskLock lock = 0);

    /// Return number of tasks in queue.
    size_t queueSize();

  private:
    void threadLoop(rThread thnead);
    Lockable lock_;
    /// Main task queue (locked by lock_).
    std::list<rTaskHandle> queue_;
    /// Cached number of locked tasks.
    size_t nLockedTasks_;

    class LIBPORT_API Thread: public ThreadSafeRefCounted
    {
    public:
      /// Must have a dtor to avoid a g++ 'sorry, unimplemented' inlining error
      ~Thread();
      pthread_t handle;
      Semaphore sem;
      rTaskHandle currentTask;
      rTaskLock  taskLock; //taskLock we currently handle
    };

    /// All threads (locked by lock_)
    std::list<rThread> threads_;
    /// Idle threads (locked by lock_)
    std::vector<rThread> idleThreads_;
    size_t maxThreads_;
  };
}

#endif
