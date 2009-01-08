#ifndef SCHED_CONFIGURATION_HH
# define SCHED_CONFIGURATION_HH

# include <cstdlib>
// # include <libport/export.hh>

namespace sched
{
  struct Configuration
  {
    /// Default and minimum stack size for jobs (in bytes)
    size_t default_stack_size;
    size_t minimum_stack_size;
  };

  extern Configuration configuration;
  // URBI_SDK_API
}

#endif // SCHED_CONFIGURATION_HH
