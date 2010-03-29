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
 ** \file sched/fwd.hh
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
