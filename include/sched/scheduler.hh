/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file sched/scheduler.hh
 ** \brief Definition of sched::Scheduler.
 */

#ifndef SCHED_SCHEDULER_HH
# define SCHED_SCHEDULER_HH

# include <iosfwd>

# include <boost/any.hpp>
# include <boost/function.hpp>
# include <libport/ufloat.hh>
# include <libport/statistics.hh>
# include <boost/utility.hpp>
# include <libport/utime.hh>

# include <sched/coroutine.hh>
# include <sched/export.hh>
# include <sched/fwd.hh>

namespace sched
{

  enum {
    SCHED_EXIT      = -1,
    SCHED_IMMEDIATE = 0
  };

  typedef libport::Statistics<libport::utime_t, libport::ufloat>
    scheduler_stats_type;

  class SCHED_API Scheduler : boost::noncopyable
  {
  public:
    /// Constructor.
    ///
    /// \param get_time A function which, when called, returns the
    ///        current system time.
    Scheduler(boost::function0<libport::utime_t> get_time);

    /// Destructor.
    ~Scheduler();

  public:
    /// Do one cycle of work, and return the next time we expect to be called.
    ///
    /// \return If we have work to do, 0 will be returned
    /// (\p SCHED_IMMEDIATE) in order to be called again as soon as
    /// possible. If we only have time-suspended or dependent jobs, we
    /// will return the time of the next scheduled one. If \p killall_jobs()
    /// has been called and all jobs are dead, \p SCHED_EXIT will be returned.
    ///
    /// Calling work() again before the returned time is useless as there will
    /// be nothing to do except if some new work has been entered in.
    libport::utime_t work();

    /// Add a job to the list of jobs to be run later.
    ///
    /// \param job Job to be started. Please do not use this function
    ///        directly except from the \c Job::start_job() method.
    ///
    /// Jobs added during a cycle will be started at the next cycle by the
    /// scheduler.
    void add_job(rJob job);

    /// Terminate all jobs. This must be called only when the executable
    /// is going to terminate.
    void killall_jobs();

    /// Resume scheduler execution.
    ///
    /// \param job Job currently being executed. This job will relinquish
    ///        the CPU to the scheduler. Note that the scheduler is free
    ///        to reschedule \a job immediately if it wishes to do so.
    void resume_scheduler(rJob job);

    /// Get the currently executing job.
    ///
    /// \return A reference onto the currently executing job.
    ///
    /// It is an error to call this method if no job is currently
    /// executing or to keep the reference after yielding since the
    /// Job may no longer be valid then.
    Job& current_job() const;

    /// Get the currently executing job, if any.
    ///
    /// \return A reference onto the currently executing job, or zero
    /// if we're not executing a job.
    Job* current_job_opt() const;

    /// Check whether a job is the current job.
    ///
    /// \param job Job to be checked against the current job.
    ///        0 can be used to check if no job is executing.
    ///
    /// \return \a true if \a job is the current job.
    bool is_current_job(const Job* job) const;

    /// Signal that a \c stop or a \c block has been issued on a tag.
    ///
    /// \param tag The tag that has been recently stopped or blocked.
    /// \param payload The data to embed in the StopException.
    ///
    /// After this function has been called, the scheduler will determine,
    /// at the end of the current cycle, which jobs need to react to this
    /// action.
    void signal_stop(const Tag& tag, const boost::any& payload);

    /// Get the current cycle number.
    ///
    /// \return The current cycle index, increasing by 1 at each cycle.
    unsigned int cycle_get() const;

    /// Get the time as seen by the scheduler.
    ///
    /// \return The current system time.
    libport::utime_t get_time() const;

    /// Get the current jobs list.
    ///
    /// \return The currently non-terminated known jobs.
    jobs_type jobs_get() const;

    /// Tell the scheduler that the world has changed because
    /// of an external event.
    ///
    void signal_world_change();

    /// Get current mean and standard deviation (in libport::utime_t units) of
    /// the scheduler so far.
    ///
    /// \return Some scheduler statistics.
    const scheduler_stats_type& stats_get() const;

    /// Reset statistics.
    void stats_reset();

    /// Indicate that we now want real-time behavior (which may be already
    /// on).
    void real_time_behavior_set();

    /// Check whether we want real-time behavior or not.
    bool real_time_behavior_get() const;

    /// Sets whether to keep the list of jobs in the terminated state
    void keep_terminated_jobs_set(bool keep);
    /// Get the list of terminated jobs
    jobs_type terminated_jobs_get() const;
    /// Clear the list of terminated jobs
    void terminated_jobs_clear();

    /// Return currently computed deadline for next round.
    libport::utime_t deadline_get() const;

    /// Set the job that gets scheduled after execute_round.
    void idle_job_set(rJob job);
  private:
    /// Execute one round in the scheduler.
    ///
    /// \return See work().
    libport::utime_t execute_round();

    /// Function to retrieve the current system time.
    boost::function0<libport::utime_t> get_time_;

    /// List of jobs we are in charge of. During a cycle execution,
    /// this is where jobs will accumulate themselves after they have
    /// been executed.
    jobs_type jobs_;

    /// List of jobs currently being scheduled during the current round.
    jobs_type pending_;

    /// Current job.
    rJob current_job_;

    rJob idle_job_;
    /// Iterator on the next job if iterating at the beginning of the
    /// iteration.
    jobs_type::iterator next_job_p_;

    /// Coroutine corresponding to the scheduler.
    Coro coro_;

    /// Has there been a possible side-effect since last time we reset
    /// this field?
    bool possible_side_effect_;

    /// Cycles counter.
    unsigned int cycle_;

    /// Ready to die when all jobs are also dead.
    bool ready_to_die_;

    /// Statistics
    scheduler_stats_type stats_;

    /// Is real-time behavior desired?
    bool real_time_behavior_;

    /// List of terminated jobs (kept if keep_terminated_jobs_ is true)
    jobs_type terminated_jobs_;
    bool keep_terminated_jobs_;

    /// Deadline for next round
    libport::utime_t deadline_;
  };

} // namespace sched

# include <sched/scheduler.hxx>

#endif // !SCHED_SCHEDULER_HH
