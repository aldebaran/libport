#ifndef LIBPORT_SCHED_HH
# define LIBPORT_SCHED_HH

namespace libport
{
  int sched_estimate_granularity();
  
  void sched_set_high_priority ();
}

#endif // !LIBPORT_SCHED_HH
