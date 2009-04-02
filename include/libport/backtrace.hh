#ifndef LIBPORT_BACKTRACE_HH
# define LIBPORT_BACKTRACE_HH

# include <vector>

# include <libport/export.hh>

# ifdef WIN32
#  include <string>
typedef std::vector<std::string> backtrace_type;
# else // !WIN32
typedef std::vector<const char*> backtrace_type;
# endif

namespace libport
{
  /// Return the stack trace as a collection of strings.
  /// There is nothing particular to do about the memory, just
  /// clear the container when done.
  LIBPORT_API
  backtrace_type backtrace();
}

#endif // !LIBPORT_BACKTRACE_HH
