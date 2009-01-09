#ifndef SCHED_CONFIGURATION_HH
# define SCHED_CONFIGURATION_HH

# include <cstdlib>
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
