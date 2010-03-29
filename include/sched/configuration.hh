/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_CONFIGURATION_HH
# define SCHED_CONFIGURATION_HH

# include <libport/cstdlib>
# include <sched/export.hh>

namespace sched
{
  struct SCHED_API Configuration
  {
    /// Default and minimum stack size for jobs (in bytes)
    size_t default_stack_size;
    size_t minimum_stack_size;
  };

  /// Default configuration.
  extern SCHED_API Configuration configuration;
}

#endif // SCHED_CONFIGURATION_HH
