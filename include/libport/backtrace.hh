#ifndef LIBPORT_BACKTRACE_HH
# define LIBPORT_BACKTRACE_HH

# include <vector>

# include <libport/export.hh>

namespace libport
{

  std::vector<const char*>
  LIBPORT_API
  backtrace();
}


#endif // !LIBPORT_BACKTRACE_HH
