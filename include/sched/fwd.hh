/**
 ** \file scheduler/fwd.hh
 ** \brief Forward declarations for the namespace sched.
 */

#ifndef SCHED_FWD_HH
# define SCHED_FWD_HH

# include <list>

# include <libport/intrusive-ptr.hh>

# include <sched/exception.hh>

namespace sched
{

  class Scheduler;
  class Job;
  typedef libport::intrusive_ptr<Job> rJob;
  typedef std::list<rJob> jobs_type;
  class Tag;
  typedef libport::intrusive_ptr<Tag> rTag;

  // This exception is above other scheduler-related exceptions such
  // as BlockedException. This allows catching more specific exceptions
  // first, then handling scheduler-related exceptions in a general
  // way.
  struct SchedulerException : public exception
  {
    COMPLETE_EXCEPTION(SchedulerException);
  };

} // namespace sched

#endif // !SCHED_FWD_HH
