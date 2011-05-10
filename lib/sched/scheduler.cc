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
 ** \file sched/scheduler.cc
 ** \brief Implementation of sched::Scheduler.
 */

#include <algorithm>
#include <libport/cassert>
#include <libport/cstdlib>

#include <libport/containers.hh>
#include <libport/deref.hh>
#include <libport/foreach.hh>

#include <sched/scheduler.hh>
#include <sched/job.hh>

Coro* coroutine_main_;
LocalCoroPtr coroutine_current_;
void (*coroutine_new_hook) (Coro*) = 0;
void (*coroutine_free_hook)(Coro*) = 0;

GD_CATEGORY(Sched);

namespace sched
{

  Scheduler::Scheduler(boost::function0<libport::utime_t> get_time)
    : get_time_(get_time)
    , current_job_(0)
    , new_job_(false)
    , awoken_job_(false)
    , cycle_(0)
    , ready_to_die_(false)
    , real_time_behavior_(false)
    , keep_terminated_jobs_(false)
  {
    GD_INFO_DUMP("Initializing main coroutine");
    coroutine_initialize_main(&coro_);
  }

  Scheduler::~Scheduler()
  {
    GD_INFO_DUMP("Destroying scheduler");
  }

  // This function is required to start a new job using the libcoroutine.
  // Its only purpose is to create the context and start the execution
  // of the new job.
  static void
  run_job(Job* job)
  {
    job->run();
  }

  void
  Scheduler::add_job(rJob job)
  {
    aver(job);
    aver(!libport::has(jobs_, job));
    aver(!libport::has(pending_, job));
    if (ready_to_die_)
      GD_WARN("add_job called on a ready to die scheduler");
    // If we are currently in a job, add it to the pending_ queue so that
    // the job is started in the course of the current round. To make sure
    // that it is not started too late even if the creator is located after
    // the job that is causing the creation (think "at job handler" for
    // example), insert it right before the job which was right after the
    // current one. This way, jobs inserted successively will get queued
    // in the right order.
    if (current_job_ && current_job_ != idle_job_)
      pending_.insert(next_job_p_, job);
    else
      jobs_.push_back(job);
    new_job_ = true;
  }

  libport::utime_t
  Scheduler::work()
  {
    ++cycle_;
    GD_FINFO_DUMP("========================================================"
                  " cycle %s", cycle_);

    libport::utime_t deadline = execute_round();
    return deadline;
  }

  void
  Scheduler::idle_job_set(rJob ijob)
  {
    idle_job_ = ijob;
    current_job_ = idle_job_;
    coroutine_start(&coro_,
                    idle_job_->coro_get(), run_job, idle_job_.get());
    current_job_ = 0;
  }

  static bool prio_gt(const rJob& j1, const rJob& j2)
  {
    return j2->prio_get() < j1->prio_get();
  }

  libport::utime_t
  Scheduler::execute_round()
  {
    // We are using a direct coro-to-coro switch.
    // Just initialize our loop variables here, all the per-job logic is in
    // switch_to_next_.

    // Run all the jobs in the run queue once.
    pending_.clear();
    std::swap(pending_, jobs_);

    // Sort all the jobs according to their priority.
    if (real_time_behavior_)
    {
      static std::vector<rJob> tmp;
      tmp.reserve(pending_.size());
      tmp.clear();
      foreach(rJob job, pending_)
	tmp.push_back(job);
      std::stable_sort(tmp.begin(), tmp.end(), prio_gt);
      pending_.clear();
      foreach(rJob job, tmp)
	pending_.push_back(job);
    }

    // By default, wake us up after one hour and consider that we have no
    // new job to start. Also, run waiting jobs only if the previous round
    // may have add a side effect and reset this indication for the current
    // job.
    start_time_ = get_time_();
    deadline_ = start_time_ +  3600000000LL;
    at_least_one_started_ = false;

    GD_FINFO_DUMP("%s jobs in the queue for this round", pending_.size());

    job_p_ = pending_.begin();
    switch_to_next_(&coro_, true);
    // When we reach here, IDLE job has already been executed.
    GD_FINFO_DUMP("Back to execute_round, nj=%s, aj=%s, die=%s, returning %d",
                  new_job_, awoken_job_, ready_to_die_, deadline_);
    new_job_ = false;
    awoken_job_ = false;
    // If we are ready to die and there are no jobs left, then die.
    if (ready_to_die_ && jobs_.empty())
      deadline_ = SCHED_EXIT;
    return deadline_;
  }

  void
  Scheduler::switch_to_next_(Coro* current_coro, bool first)
  {
    /* We are using a direct coro-to-coro switch now. Which means code after
     * the coro_switch_to line is not executed immediately when the coro
     * returns. So all locations with a coro_switch_to are returning immediately
     * after, to the caller resume_scheduler().
     *
     * Care must be taken to not hold any job ref when switching, we may never
     * come back if the current job was terminated.
     */
    GD_FINFO_DUMP("switch_to_next from %s (%s)", current_coro, first);
    // To simplify, idle_job_ is also yielding through this function.
    if (idle_job_ && current_coro == idle_job_->coro_get())
    {
      coroutine_switch_to(current_coro, &coro_);
      return;
    }
    while (true)
    {
      if (!first)
        ++job_p_;
      first = false;
      // If we are done, return to scheduler main coro
      if (job_p_ == pending_.end())
        break;
      rJob job = *job_p_;
      // Store the next job so that insertions happen before it.
      next_job_p_ = job_p_;
      ++next_job_p_;
      // If the job has terminated during the previous round, remove the
      // references we have on it by just skipping it.
      if (job->terminated())
        continue;

      // Should the job be started?
      bool start = false;

      // Save the current time since we will use it several times
      // during this job analysis.
      libport::utime_t current_time = get_time_();

      GD_FINFO_DUMP("Considering %s in state %s", *job, job->state_get());

      switch (job->state_get())
      {
      case to_start:
      {
        // New job. Start its coroutine but do not start the job as it
        // would be queued twice otherwise. It will start doing real
        // work at the next cycle, so set deadline to 0. Note that we
        // use "continue" here to avoid having the job requeued
        // because it hasn't been started by setting "start".
        //
        // The code below takes care of destroying the rJob reference
        // to the job, so that it does not stay in the call stack as a
        // local variable. If it did, the job would never be
        // destroyed. However, to prevent the job from being
        // prematurely destroyed, we set current_job_ (global to the
        // scheduler) to the rJob.
	GD_FINFO_DUMP("Job %s is starting", *job);
        current_job_ = job;
	job = 0;
        deadline_ = SCHED_IMMEDIATE;
	at_least_one_started_ = true;
	coroutine_start(current_coro,
                        current_job_->coro_get(), run_job, current_job_.get());
        GD_INFO_DUMP("Back at #2, returning from switch_to_next_");
	return;
      }
      case zombie:
	pabort("zombie");
	break;
      case running:
	start = true;
	break;
      case sleeping:
	{
	  libport::utime_t job_deadline = job->deadline_get();

	  // If the job has been frozen, extend its deadline by the
	  // corresponding amount of time. The deadline will be adjusted
	  // later when the job is unfrozen using notice_not_frozen(),
	  // but in the meantime we do not want to cause an early wakeup.
	  if (libport::utime_t frozen_since = job->frozen_since_get())
	    job_deadline += current_time - frozen_since;

	  if (job_deadline <= current_time)
	    start = true;
	  else
	    deadline_ = std::min(deadline_, job_deadline);
	}
	break;
      case waiting:
	// Since jobs keep their orders in the queue, start waiting jobs if
	// previous jobs in the run have had a possible side effect or if
	// the previous run may have had some. Without it, we may miss some
	// changes if the watching job is after the modifying job in the queue
	// and the watched condition gets true for only one cycle.
	start =  !job->frozen() || job->has_pending_exception();
	break;
      case joining:
	break;
      }

      // Tell the job whether it is frozen or not so that it can remember
      // since when it has been in this state.
      if (job->frozen())
	job->notice_frozen(current_time);
      else
	job->notice_not_frozen(current_time);

      // A job with an exception will start unconditionally.
      if (start || job->has_pending_exception())
      {
	at_least_one_started_ = true;
	GD_FINFO_DUMP("will resume job %s", *job);
        current_job_ = job;
        job = 0;
	coroutine_switch_to(current_coro, current_job_->coro_get());
        GD_INFO_DUMP("Back at #3, returning from switch_to_next_");
        return;
      }
      else
	jobs_.push_back(job);   // Job not started, keep it in queue
    }
    GD_FINFO_DUMP("Round finished, back to main coro (switch = %s)",
                  current_coro != &coro_);
    current_job_ = 0;


    // If during this cycle a new job has been created by an existing job,
    // start it.
    // If during this cycle a job has been marked running behind our back, rerun
    // immediately.
    // Same thing if we are ready to die, finish the job ASAP.
    if (new_job_ || awoken_job_ || ready_to_die_)
      deadline_ = SCHED_IMMEDIATE;
    GD_INFO_DUMP(deadline_
                 ? libport::format("Scheduler asking to be woken up in %ss",
                                   (double)(deadline_ - get_time_())
                                     / 1000000.0)
                 : "Scheduler asking to be woken up ASAP");
    // Compute statistics
    if (at_least_one_started_)
      stats_.add_sample(get_time_() - start_time_);
    if (idle_job_)
    {
      coroutine_switch_to(current_coro,  idle_job_->coro_get());
      current_job_ = 0;
    }
    else if (current_coro != &coro_)
      coroutine_switch_to(current_coro, &coro_);
    GD_INFO_DUMP("Back at #1, returning from switch_to_next_");
  }

  void
  Scheduler::resume_scheduler(rJob job)
  {
    if (job->state_get() == running
        && UPRIO_RT_MIN <= job->prio_get()
        && !job->frozen())
      return;

    // We may have to suspend the job several time in case it makes no sense
    // to start it back. Let's do it in a loop and we'll break when we want
    // to resume the job.

    while (true)
    {
      // Add the job at the end of the scheduler queue unless the job has
      // already terminated.
      if (job != idle_job_)
      {
        if (!job->terminated())
	  jobs_.push_back(job);
        else if (keep_terminated_jobs_)
          terminated_jobs_.push_back(job);
      }


      // Switch back to the scheduler. But in the case this job has been
      // destroyed, erase the local variable first so that it doesn't keep
      // a reference on it which will never be destroyed.
      aver(current_job_ == job);
      GD_FINFO_DUMP("%s has %sterminated (state: %s)",
                    *job, job->terminated() ? "" : "not ", job->state_get());
      Coro* current_coro = job->coro_get();
      if (job->terminated())
	job = 0;
      else
        switch (job->state_get())
	{
	case running:
	  deadline_ = SCHED_IMMEDIATE;
	  break;
	case sleeping:
	  deadline_ = std::min(deadline_, job->deadline_get());
	  break;
	default:
	  break;
	}
      // Calling switch_to_next_ from IDLE job is ok
        switch_to_next_(current_coro);

      // If we regain control, we are not dead.
      aver(job);

      // We regained control, we are again in the context of the job.
      current_job_ = job;
      GD_FINFO_DUMP("job %s resumed", *job);

      // Execute a deferred exception if any; this may break out of this loop
      job->check_for_pending_exception();

      // If we are not frozen, it is time to resume regular execution
      if (!job->frozen())
	break;

      // Ok, we are frozen. Let's requeue ourselves,
      // we will be in waiting mode.

      job->state_set(waiting);
    }

    // Check that we are not near exhausting the stack space.
    job->check_stack_space();

    // Resume job execution
  }

  void
  Scheduler::killall_jobs()
  {
    // Mark the scheduler as ready to die when all the jobs are
    // really dead.
    ready_to_die_ = true;

    // Killing the current job (the one requesting the termination)
    // will result in its immediate termination (including its
    // children), so it won't be given a chance to kill the unrelated
    // jobs.
    //
    // So first kill all jobs which death will not terminate this one.
    //
    // To this end, we first find the oldest ancester of the
    // current_job_, they we kill all the jobs that do not descend
    // from it, then we kill it.
    rJob ancester = 0;
    if (current_job_)
    {
      ancester = current_job_;
      // FIXME: How can this work?  What guarantee do we have to have
      // the oldest ancester?
      foreach (const rJob& job, jobs_get())
        if (job->ancester_of(ancester))
          ancester = job;
    }

    foreach (const rJob& job, jobs_get())
      if (!ancester || !ancester->ancester_of(job))
	job->terminate_now();

    if (ancester)
    {
      // If the ancester is just the current job, we can kill it now.
      // Otherwise, since terminate_now first kills the children, the
      // current_job_ cannot kill its ancester, so we send a signal to
      // it, and wait for the signal to terminate the ancester and its
      // family.
      if (ancester == current_job_)
        ancester->terminate_now();
      else
        ancester->terminate_asap();
    }
  }

  void
  Scheduler::signal_stop(const Tag& tag, const boost::any& payload)
  {
    // Tell the jobs that a tag has been stopped, ending with
    // the current job to avoid interrupting this method early.
    foreach (const rJob& job, jobs_get())
    {
      // The current job will be handled last.
      if (job == current_job_)
	continue;
      // Job to be started during this cycle.
      if (job->state_get() == to_start)
      {
	// Check if this job deserves to be removed.
	if (job->has_tag(tag))
	{
	  pending_.remove(job);
	  continue;
	}
      }
      // Any other job.
      else
	job->register_stopped_tag(tag, payload);
    }
    // Handle the current job situation.
    if (current_job_)
      current_job_->register_stopped_tag(tag, payload);
  }

  jobs_type
  Scheduler::jobs_get() const
  {
    // If this method is called from within a job, return the currently
    // executing jobs (pending_), otherwise return the jobs_ content which
    // is complete.
    return current_job_ ? pending_ : jobs_;
  }

  const scheduler_stats_type&
  Scheduler::stats_get() const
  {
    return stats_;
  }

  void
  Scheduler::stats_reset()
  {
    stats_.resize(0);
  }

} // namespace sched
