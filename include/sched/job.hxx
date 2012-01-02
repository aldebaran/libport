/*
 * Copyright (C) 2009-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file sched/job.hxx
 ** \brief Inline implementation of Job.
 */

#ifndef SCHED_JOB_HXX
# define SCHED_JOB_HXX

# include <libport/bind.hh>
# include <libport/cassert>
# include <libport/debug.hh>

# include <sched/scheduler.hh>
# include <sched/coroutine.hh>

namespace sched
{
  /*------------.
  | job_state.  |
  `------------*/

  inline const char*
  name(job_state state)
  {
#define CASE(State) case State: return #State;
    switch (state)
    {
      CASE(to_start)
      CASE(running)
      CASE(sleeping)
      CASE(waiting)
      CASE(joining)
      CASE(zombie)
    }
#undef CASE
    return "<unknown state>";
  }

  inline
  std::ostream&
  operator<<(std::ostream& o, job_state s)
  {
    return o << name(s);
  }


  /*------.
  | Job.  |
  `------*/

  inline void
  Job::init_common(size_t stack_size)
  {
    state_ = to_start;
    frozen_since_ = 0;
    time_shift_ = 0;
    coro_ = coroutine_new(stack_size);
    non_interruptible_ = false;
    check_stack_space_ = stack_size == 0;
    alive_jobs_++;
  }

  inline
  Job::Job(Scheduler& scheduler)
    : RefCounted()
    , scheduler_(scheduler)
    , stats_()
  {
    init_common();
  }

  inline
  Job::Job(const Job& model, size_t stack_size)
    : RefCounted()
    , scheduler_(model.scheduler_)
    , stats_()
  {
    init_common(stack_size);
    time_shift_ = model.time_shift_;
  }

  inline
  Job::~Job()
  {
    aver(children_.empty(), children_);
    coroutine_free(coro_);
    alive_jobs_--;
  }

  inline Scheduler&
  Job::scheduler_get() const
  {
    return scheduler_;
  }

  inline bool
  Job::terminated() const
  {
    return state_ == zombie;
  }

  inline void
  Job::yield()
  {
    // The only case where we yield while being non-interruptible
    // is when we get frozen. This is used to suspend a task
    // and resume it in non-interruptible contexts.
    if (non_interruptible_ && !frozen())
      return;

    state_ = running;
    resume_scheduler_();
  }

  inline void
  Job::yield_until(libport::utime_t deadline)
  {
    if (non_interruptible_)
      scheduling_error("attempt to sleep in non-interruptible code");

    state_ = sleeping;
    deadline_ = deadline;
    resume_scheduler_();
  }

  inline void
  Job::yield_for(libport::utime_t delay)
  {
    yield_until(scheduler_.get_time() + delay);
  }

  inline Coro*
  Job::coro_get() const
  {
    aver(coro_);
    return coro_;
  }

  inline void
  Job::start_job()
  {
    aver(state_ == to_start);
    scheduler_.add_job(this);
  }

  inline bool
  Job::non_interruptible_get() const
  {
    return non_interruptible_;
  }

  inline void
  Job::non_interruptible_set(bool ni)
  {
    non_interruptible_ = ni;
  }

  inline void
  Job::check_stack_space()
  {
    GD_CATEGORY(Sched.Job);
    if (check_stack_space_
	&& coroutine_stack_space_almost_gone(coro_))
    {
      // Cannot be nicely converted to using FINALLY because of
      // MSVC2005.  See finally.hh.
      libport::Finally finally(libport::scoped_set(check_stack_space_, false));
      GD_ERROR("Stack space exhausted");
      scheduling_error("stack space exhausted");
    }
  }

  inline job_state
  Job::state_get() const
  {
    return state_;
  }

  inline void
  Job::state_set(job_state state)
  {
    state_ = state;
    if (state_ == running)
      scheduler_get().job_was_woken_up();
  }

  inline libport::utime_t
  Job::deadline_get() const
  {
    return deadline_;
  }

  inline void
  Job::notice_frozen(libport::utime_t current_time)
  {
    if (!frozen_since_)
      frozen_since_ = current_time;
  }

  inline void
  Job::notice_not_frozen(libport::utime_t current_time)
  {
    if (frozen_since_)
    {
      libport::utime_t time_spent_frozen = current_time - frozen_since_;
      time_shift_ += time_spent_frozen;
      if (state_ == sleeping)
	deadline_ += time_spent_frozen;
    }
    frozen_since_ = 0;
  }

  inline libport::utime_t
  Job::frozen_since_get() const
  {
    return frozen_since_;
  }

  inline libport::utime_t
  Job::time_shift_get() const
  {
    return time_shift_;
  }

  inline void
  Job::time_shift_set(libport::utime_t ts)
  {
    time_shift_ = ts;
  }

  inline bool
  Job::has_pending_exception() const
  {
    return pending_exception_.get();
  }

  inline bool
  Job::child_job() const
  {
    return parent_;
  }

  inline
  bool
  Job::ancester_of(const rJob& that) const
  {
    for (const Job* j = that.get(); j; j = j->parent_.get())
      if (j == this)
        return true;
    return false;
  }

  inline std::ostream&
  operator<< (std::ostream& o, const Job& j)
  {
    return j.dump(o);
  }

  inline void
  Job::resume_scheduler_()
  {
    hook_preempted();

    job_state last_state = state_;
    if (frozen())
    {
      last_state = waiting;
      if (non_interruptible_)
        scheduling_error("attempt to wait in non-interruptible code");
    }

    // Ensure that a non-interruptible job does not let other job scheduled
    // between the beginning and the end.
    aver(!non_interruptible_);

    if (stats_.logging)
    {
      libport::utime_t start_resume = scheduler_.get_time();
      stats_.job.running.add_sample(
        start_resume - stats_.last_resume);

      scheduler_.resume_scheduler(this);

      stats_.last_resume = scheduler_.get_time();
      switch (last_state)
      {
        case waiting:
          stats_.job.waiting.add_sample(stats_.last_resume - start_resume);
          break;
        case sleeping:
          stats_.job.sleeping.add_sample(stats_.last_resume - start_resume);
          break;
        default:
          break;
      }
    }
    else
      scheduler_.resume_scheduler(this);
    hook_resumed();
  }

  /*--------.
  | Stats.  |
  `--------*/

  inline
  const Job::stats_type&
  Job::stats_get() const
  {
    return stats_;
  }

  inline
  Job::stats_type&
  Job::stats_get()
  {
    return stats_;
  }

  inline
  Job::stats_type::stats_type()
    : logging(true)
  {
    job.reset();
    terminated_children.reset();
  }

  inline void
  Job::stats_type::thread_stats_type::reset()
  {
    running.resize(0);
    waiting.resize(0);
    sleeping.resize(0);
    nb_fork = 0;
    nb_join = 0;
    nb_exn = 0;
  }

  inline void
  Job::stats_reset()
  {
    stats_.job.reset();
    stats_.terminated_children.reset();
  }

  inline void
  Job::stats_log(bool log)
  {
    stats_.logging = log;
    if (log)
      stats_.last_resume = scheduler_.get_time();
  }

  /*-----------------.
  | ChildException.  |
  `-----------------*/

  inline
  ChildException::ChildException(exception_ptr exc)
    : child_exception_(exc)
  {
  }

  inline
  ChildException::ChildException(const ChildException& exc)
    : SchedulerException(exc)
    , child_exception_(exc.child_exception_)
  {
  }

  inline exception_ptr
  ChildException::clone() const
  {
    return exception_ptr(new ChildException(child_exception_));
  }

  inline void
  ChildException::rethrow_() const
  {
    throw *this;
  }

  inline void
  ChildException::rethrow_child_exception() const
  {
    child_exception_->rethrow();
    abort(); // Help clang++ 2.1.
  }


  /*------------.
  | Collector.  |
  `------------*/

  ATTRIBUTE_ALWAYS_INLINE
  Job::Collector::Collector(rJob parent, size_t)
    : super_type()
    , parent_(parent)
  {
  }

} // namespace sched

#endif // !SCHED_JOB_HXX
