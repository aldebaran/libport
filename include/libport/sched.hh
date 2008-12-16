#ifndef LIBPORT_SCHED_HH
# define LIBPORT_SCHED_HH

# include <libport/export.hh>

namespace libport
{
  LIBPORT_API int sched_estimate_granularity();

  LIBPORT_API void sched_set_high_priority ();
}

#endif // !LIBPORT_SCHED_HH
