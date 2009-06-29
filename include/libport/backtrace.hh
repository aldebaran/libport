#ifndef LIBPORT_BACKTRACE_HH
# define LIBPORT_BACKTRACE_HH

# include <vector>

# include <libport/export.hh>
# include <string>

namespace libport
{
  /// Storing the description of the stack trace.
  /// Used to be a vector<char*>, but was wrong, because the
  /// array of char* was malloced as a whole, and freeing it
  /// not only frees the array, but the char* too.  So duplicated
  /// everything.
  typedef std::vector<std::string> backtrace_type;

  /// Return the stack trace as a collection of strings.
  /// There is nothing particular to do about the memory, just
  /// clear the container when done.
  LIBPORT_API
  backtrace_type backtrace();
}

#endif // !LIBPORT_BACKTRACE_HH
