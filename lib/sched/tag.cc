/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstdlib>

#include <sched/scheduler.hh>
#include <sched/tag.hh>

namespace sched
{
  void
  Tag::stop(Scheduler& sched, const boost::any& payload) const
  {
    stop_hook_();
    sched.signal_stop(*this, payload);
  }

  const boost::any&
  Tag::payload_get() const
  {
    aver(blocked_);
    return payload_;
  }

  boost::signal0<void>&
  Tag::stop_hook_get()
  {
    return stop_hook_;
  }

  boost::signal0<void>&
  Tag::freeze_hook_get()
  {
    return freeze_hook_;
  }

  boost::signal0<void>&
  Tag::unfreeze_hook_get()
  {
    return unfreeze_hook_;
  }

} // namespace sched
