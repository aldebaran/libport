/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SCHED_HH
# define LIBPORT_SCHED_HH

# include <libport/export.hh>

namespace libport
{
  LIBPORT_API int sched_estimate_granularity();

  LIBPORT_API void sched_set_high_priority ();
}

#endif // !LIBPORT_SCHED_HH
