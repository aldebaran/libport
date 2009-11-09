/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
/**
 ** \file sched/scheduler.hxx
 ** \brief Inline implementation of sched::Scheduler.
 */

#ifndef SCHED_SCHEDULER_HXX
# define SCHED_SCHEDULER_HXX

# include <sched/scheduler.hh>
# include <sched/job.hh>

namespace sched
{
  inline Job&
  Scheduler::current_job() const
  {
    assert(current_job_);
    return *current_job_;
  }

  inline bool
  Scheduler::is_current_job(const Job* job) const
  {
    return current_job_ == job;
  }

  inline unsigned int
  Scheduler::cycle_get() const
  {
    return cycle_;
  }

  inline libport::utime_t
  Scheduler::get_time() const
  {
    return get_time_();
  }

  inline void
  Scheduler::signal_world_change()
  {
    possible_side_effect_ = true;
  }

  inline void
  Scheduler::real_time_behaviour_set()
  {
    real_time_behaviour_ = true;
  }

  inline bool
  Scheduler::real_time_behaviour_get() const
  {
    return real_time_behaviour_;
  }

} // namespace sched

#endif // !SCHED_SCHEDULER_HXX
