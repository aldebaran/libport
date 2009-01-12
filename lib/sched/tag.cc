#include <cstdlib>

#include <sched/scheduler.hh>
#include <sched/tag.hh>

namespace sched
{
  void
  Tag::stop(Scheduler& sched, const boost::any& payload) const
  {
    sched.signal_stop(*this, payload);
  }

  const boost::any&
  Tag::payload_get() const
  {
    if (blocked_)
      return payload_;
    // This is an internal error and can never happen.
    pabort("!blocked");
  }

} // namespace sched
