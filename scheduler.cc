/**
 ** \file scheduler/scheduler.cc
 ** \brief Implementation of scheduler::Scheduler.
 */

//#define ENABLE_DEBUG_TRACES

#include <cassert>
#include <cstdlib>

#include <libport/compiler.hh>
#include <libport/containers.hh>
#include <libport/foreach.hh>

#include "kernel/userver.hh"

#include "object/urbi-exception.hh"

#include "scheduler/scheduler.hh"
#include "scheduler/job.hh"

namespace scheduler
{

  // This function is required to start a new job using the libcoroutine.
  // Its only purpose is to create the context and start the execution
  // of the new job.
  static void
  run_job (void* job)
  {
    static_cast<Job*>(job)->run();
  }

  void
  Scheduler::add_job (Job* job)
  {
    assert (job);
    assert (!libport::has (jobs_, job));
    jobs_.push_back (job);
    jobs_to_start_ = true;
  }

  libport::utime_t
  Scheduler::work ()
  {
#ifdef ENABLE_DEBUG_TRACES
    static int cycle = 0;
#endif
    ECHO ("======================================================== cycle "
	  << ++cycle);

    libport::utime_t deadline = execute_round (false);

    // If some jobs need to be stopped, do it as soon as possible.
    deadline = std::min (deadline, check_for_stopped_tags (deadline));

#ifdef ENABLE_DEBUG_TRACES
    if (deadline)
      ECHO ("Scheduler asking to be woken up in "
	    << (deadline - ::urbiserver->getTime ()) / 1000000L << " seconds");
    else
      ECHO ("Scheduler asking to be woken up ASAP");
#endif

    return deadline;
  }

  libport::utime_t
  Scheduler::execute_round (bool blocked_only)
  {
    // Run all the jobs in the run queue once. If any job declares upon entry or
    // return that it is not side-effect free, we remember that for the next
    // cycle.
    pending_.clear ();
    std::swap (pending_, jobs_);

    // By default, wake us up after one hour and consider that we have no
    // new job to start. Also, run waiting jobs only if the previous round
    // may have add a side effect and reset this indication for the current
    // job.
    libport::utime_t deadline = ::urbiserver->getTime () +  3600000000LL;
    jobs_to_start_ = false;
    bool start_waiting = possible_side_effect_;
    possible_side_effect_ = false;

    ECHO (pending_.size() << " jobs in the queue for this round");
    foreach (Job* job, pending_)
    {
      // Kill a job if needed. See explanation in job.hh.
      to_kill_ = 0;

      assert (job);
      assert (!job->terminated ());

      // Should the job be started?
      bool start = false;

      ECHO ("Considering " << *job << " in state " << state_name (job->state_get ()));

      switch (job->state_get ())
      {
      case to_start:
	// New job. Start its coroutine but do not start the job as it would be queued
	// twice otherwise. It will start doing real work at the next cycle, so set
	// deadline to 0. Note that we use "continue" here to avoid having the job
	// requeued because it hasn't been started by setting "start".
	ECHO ("Starting job " << *job);
	current_job_ = job;
	Coro_startCoro_ (coro_, job->coro_get(), job, run_job);
	current_job_ = 0;
	ECHO ("Job " << *job << " has been started");
	assert (job->state_get () != to_start);
	deadline = 0;
	continue;
      case zombie:
	assert (false);
	break;
      case running:
	start = !blocked_only || job->blocked ();
	break;
      case sleeping:
	{
	  libport::utime_t job_deadline = job->deadline_get ();
	  if (job_deadline <= ::urbiserver->getTime ())
	    start = true;
	  else
	    deadline = std::min (deadline, job_deadline);
	}
	break;
      case waiting:
	// Since jobs keep their orders in the queue, start waiting jobs if
	// previous jobs in the run have had a possible side effect or if
	// the previous run may have had some. Without it, we may miss some
	// changes if the watching job is after the modifying job in the queue
	// and the watched condition gets true for only one cycle.
	start = start_waiting | possible_side_effect_;
	break;
      case joining:
	break;
      }

      if (start)
      {
	ECHO ("will resume job " << *job
	      << (job->side_effect_free_get() ? " (side-effect free)" : ""));
	possible_side_effect_ |= !job->side_effect_free_get ();
	assert (!current_job_);
	Coro_switchTo_ (coro_, job->coro_get ());
	assert (!current_job_);
	possible_side_effect_ |= !job->side_effect_free_get ();
	ECHO ("back from job " << *job
	      << (job->side_effect_free_get() ? " (side-effect free)" : ""));
	switch (job->state_get ())
	{
	case running:
	  deadline = 0;
	  break;
	case sleeping:
	  deadline = std::min (deadline, job->deadline_get ());
	  break;
	default:
	  break;
	}
      }
      else
	jobs_.push_back (job);   // Job not started, keep it in queue
    }

    // Kill a job if needed. See explanation in job.hh.
    to_kill_ = 0;

    /// If during this cycle a new job has been created by an existing job,
    /// start it.
    if (jobs_to_start_)
      deadline = 0;

    return deadline;
  }

  libport::utime_t
  Scheduler::check_for_stopped_tags (libport::utime_t old_deadline)
  {
    bool blocked_job = false;

    // If we have had no stopped tag, return immediately.
    if (stopped_tags_.empty ())
      return old_deadline;

    // If some jobs have been blocked, mark them as running so that they will
    // handle the condition when they are resumed.
    foreach (Job* job, jobs_)
      if (job->blocked ())
      {
	job->state_set (running);
	blocked_job = true;
      }

    // Wake up blocked jobs.
    libport::utime_t deadline = execute_round (true);

    // Reset tags to their real blocked value and reset the list.
    foreach (tag_state_type t, stopped_tags_)
      t.first->set_blocked (t.second);
    stopped_tags_.clear ();

    return deadline;
  }

  void
  Scheduler::switch_back (Job* job)
  {
    // Switch back to the scheduler.
    assert (current_job_ == job);
    current_job_ = 0;
    Coro_switchTo_ (job->coro_get (), coro_);
    // We regained control, we are again in the context of the job.
    assert (!current_job_);
    current_job_ = job;
    ECHO ("job " << *job << " resumed");
    // Check that we are not near exhausting the stack space.
    job->check_stack_space ();
    // Execute a deferred exception if any
    job->check_for_pending_exception ();
    // If we are in frozen state, let's requeue ourselves waiting
    // for something to change. And let's mark us side-effect
    // free during this time so that we won't cause other jobs
    // to be scheduled. Of course, we have to check for pending
    // exceptions each time we are woken up.
    if (job->frozen ())
    {
      bool side_effect_free_save = job->side_effect_free_get ();
      do {
	job->side_effect_free_set (true);
	job->yield_until_things_changed ();
	job->side_effect_free_set (side_effect_free_save);
	// Execute a deferred exception if any
	job->check_for_pending_exception ();
      } while (job->frozen ());
    }
  }

  void
  Scheduler::resume_scheduler (Job* job)
  {
    // If the job has not terminated and is side-effect free, then we
    // assume it will not take a long time as we are probably evaluating
    // a condition. In order to reduce the number of cycles spent to evaluate
    // the condition, continue until it asks to be suspended in another
    // way or until it is no longer side-effect free.

    if (!job->terminated ())
      {
	if (job->state_get () == running && job->side_effect_free_get ())
	  return;
	else
	  jobs_.push_back (job);
      }

    ECHO (*job << " has " << (job->terminated () ? "" : "not ") << "terminated\n\t"
	  << "state: " << state_name (job->state_get ()));
    switch_back (job);
  }

  void
  Scheduler::killall_jobs ()
  {
    ECHO ("killing all jobs!");

    foreach (Job* job, jobs_)
      kill_job (job);

    foreach (Job* job, pending_)
      kill_job (job);
  }

  void
  Scheduler::unschedule_job (Job* job)
  {
    assert (job);
    assert (job != current_job_);

    ECHO ("unscheduling job " << *job);

    // Remove the job from the queue.
    jobs_.remove (job);

    // Remove it from live queues as well if the job is destroyed.
    pending_.remove (job);
  }

  void
  Scheduler::kill_job (Job* job)
  {
    KillException ke;
    job->async_throw (ke);
  }

  void Scheduler::signal_stop (rTag t)
  {
    bool previous_state = t->own_blocked ();
    t->set_blocked (true);
    stopped_tags_.push_back (std::make_pair(t, previous_state));
  }

} // namespace scheduler
