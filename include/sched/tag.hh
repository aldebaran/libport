/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_TAG_HH
# define SCHED_TAG_HH

# include <boost/any.hpp>
# include <boost/signals.hpp>

# include <libport/attributes.hh>
# include <libport/finally.hh>
# include <libport/symbol.hh>

# include <sched/export.hh>
# include <sched/fwd.hh>

namespace sched
{
  /// Tag priorities
  typedef unsigned int prio_type;

  // The following names stay away from PRIO_* which is used in
  // sys/resource.h under OSX.

  enum
  {
    /// Unused for priority computations.
    UPRIO_NONE = 0,
    /// Minimum non-real-time priority.
    UPRIO_MIN = UPRIO_NONE,
    /// Default job priority when none is given.
    UPRIO_DEFAULT = UPRIO_MIN + 1,
    /// Lowest real-time priority.
    UPRIO_RT_MIN = UPRIO_DEFAULT + 1,
    /// Highest real-time priority.
    UPRIO_RT_MAX = 16,
    /// Highest priority.
    UPRIO_MAX = UPRIO_RT_MAX
  };

  // A Tag is an entity attached to zero or more scheduler jobs. Each job
  // can have zero or more tags. When a new job is created, it usually
  // inherits the tags from its creator.
  //
  // The Tag state is represented by two parameters:
  //
  //   - frozen: jobs with this tag are not supposed to do any useful work;
  //
  //   - blocked: jobs with this tag are supposed to rewind their call
  //     stack until they die or until they are no longer affected by
  //     the tag.
  //
  // Stopping a tag is an immediate operation: all the jobs holding this
  // tag must act as if they were blocked, but only once. For example, they
  // must rewind their call stack in order not to be blocked anymore, but
  // when they resume execution, if they get the same tag again, they will
  // not act as if they were blocked again.

  class SCHED_API Tag: public libport::RefCounted
  {
  public:
    // Create a new tag.
    explicit Tag(const std::string& name);
    virtual ~Tag();

    // Is this tag directly or indirectly frozen or blocked?
    bool frozen() const;
    bool blocked() const;

    // If the tag is blocked, what is its payload? It is a fatal error
    // to call this method if the tag is not blocked.
    const boost::any& payload_get() const;

    // Set and get the priority. When setting the priority, cap it with
    // the minimum and maximum values and return the chosen one.
    prio_type prio_set(Scheduler&, prio_type);
    prio_type prio_get() const;

    // Set and get the flow_control property.
    void flow_control_set();
    virtual bool flow_control_get() const;

    // Act on a tag and make the scheduler take it into account
    void freeze();
    void unfreeze();
    void block(Scheduler&, const boost::any&);
    void unblock();
    void stop(Scheduler&, const boost::any&) const;

    const std::string& name_get() const;
    void name_set(const std::string&);
    boost::signal0<void>& stop_hook_get();
    boost::signal0<void>& freeze_hook_get();
    boost::signal0<void>& unfreeze_hook_get();

  private:
    explicit Tag(const Tag&);

    bool blocked_;
    bool frozen_;
    std::string name_;
    boost::any payload_;
    prio_type prio_;
    bool flow_control_;
    boost::signal0<void> stop_hook_;
    boost::signal0<void> freeze_hook_;
    boost::signal0<void> unfreeze_hook_;
  };

} // namespace sched

# include <sched/tag.hxx>

#endif // SCHED_TAG_HH
