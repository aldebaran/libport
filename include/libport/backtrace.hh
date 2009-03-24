#ifndef LIBPORT_BACKTRACE_HH
# define LIBPORT_BACKTRACE_HH

# include <vector>

# include <libport/export.hh>

# ifdef WIN32

#  include <string>
typedef std::string backtrace_type;

# else /* WIN32 */

typedef const char * backtrace_type;

# endif

namespace libport
{
  std::vector<backtrace_type>
  LIBPORT_API
  backtrace();
}



#endif // !LIBPORT_BACKTRACE_HH
