#ifndef LIBPORT_CLI_HH
# define LIBPORT_CLI_HH

# include <string>

# include "libport/compiler.hh"

// Hopefully some day we will use Boost.ProgramOptions and get rid of
// this junk.  Yet, better share the junk than duplicate it.

namespace libport
{
  ATTRIBUTE_NORETURN
  void usage_error (const std::string& opt, const std::string& err);

  ATTRIBUTE_NORETURN
  void required_argument (const std::string& opt);

  ATTRIBUTE_NORETURN
  void missing_argument (const std::string& opt);

  ATTRIBUTE_NORETURN
  void invalid_option (const std::string& opt);

  ATTRIBUTE_NORETURN
  void invalid_option_val (const std::string& opt, const std::string& arg);

  // Can be used on a const char* to simply test for argument presence.
  template<typename T>
  T convert_argument (const std::string& opt, const std::string& arg);
}

# include "libport/cli.hxx"

#endif // !LIBPORT_CLI_HH
