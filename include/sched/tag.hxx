/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_TAG_HXX
# define SCHED_TAG_HXX

# include <libport/bind.hh>

# include <sched/scheduler.hh>

namespace sched
{

  inline
  Tag::Tag(const std::string& name)
    : blocked_(false)
    , frozen_(false)
    , name_(name)
    , prio_(UPRIO_DEFAULT)
    , flow_control_(false)
  {
  }

  inline
  Tag::~Tag()
  {
  }

  inline bool
  Tag::frozen() const
  {
    return frozen_;
  }

  inline bool
  Tag::blocked() const
  {
    return blocked_;
  }

  inline void
  Tag::freeze()
  {
    if (frozen_)
      return;
    frozen_ = true;
    freeze_hook_();
  }

  inline void
  Tag::unfreeze()
  {
    if (!frozen_)
      return;
    frozen_ = false;
    unfreeze_hook_();
  }

  inline void
  Tag::block(Scheduler& sched, const boost::any& payload)
  {
    // Blocking a blocked tag is possible, since blocking is not only changing
    // state but also an action (stop).
    blocked_ = true;
    payload_ = payload;
    stop(sched, payload);
  }

  inline void
  Tag::unblock()
  {
    payload_ = 0;
    blocked_ = false;
  }

  inline const std::string&
  Tag::name_get() const
  {
    return name_;
  }

  inline void
  Tag::name_set(const std::string& name)
  {
    name_ = name;
  }

  inline prio_type
  Tag::prio_get() const
  {
    return prio_;
  }

  inline prio_type
  Tag::prio_set(Scheduler& sched, prio_type prio)
  {
    if (prio >= UPRIO_RT_MIN)
      sched.real_time_behavior_set();
    prio_ = std::min(std::max(prio, prio_type(UPRIO_MIN)),
		     prio_type(UPRIO_MAX));
    return prio_;
  }

  inline void
  Tag::flow_control_set()
  {
    flow_control_ = true;
  }

  inline bool
  Tag::flow_control_get() const
  {
    return flow_control_;
  }

} // namespace sched

#endif // SCHED_TAG_HXX
