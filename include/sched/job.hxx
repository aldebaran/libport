/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
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
  Job::init_common(const std::string& name)
  {
    state_ = to_start;
    frozen_since_ = 0;
    time_shift_ = 0;
    name_ = name;
    coro_ = coroutine_new();
    non_interruptible_ = false;
    side_effect_free_ = false;
    check_stack_space_ = true;
    alive_jobs_++;
  }

  inline
  Job::Job(Scheduler& scheduler, const std::string& name)
    : RefCounted()
    , scheduler_(scheduler)
  {
    init_common(name);
  }

  inline
  Job::Job(const Job& model, const std::string& name)
    : RefCounted()
    , scheduler_(model.scheduler_)
  {
    init_common(name);
    time_shift_ = model.time_shift_;
  }

  inline
  Job::~Job()
  {
    passert(children_, children_.empty());
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

  inline void
  Job::side_effect_free_set(bool s)
  {
    side_effect_free_ = s;
  }

  inline bool
  Job::side_effect_free_get() const
  {
    return side_effect_free_;
  }

  inline const std::string&
  Job::name_get() const
  {
    return name_;
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
    scheduler_.resume_scheduler(this);
    hook_resumed();
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
