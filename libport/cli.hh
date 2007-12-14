#ifndef LIBPORT_CLI_HH
# define LIBPORT_CLI_HH

# include <string>

# include "libport/compiler.hh"

// Hopefully some day we will use Boost.ProgramOptions and get rid of
// this junk.  Yet, better share the junk than duplicate it.

namespace libport
{
  void usage_error (const std::string& opt, const std::string& err)
    __attribute__ ((__noreturn__));

  void required_argument (const std::string& opt)
    __attribute__ ((__noreturn__));

  void missing_argument (const std::string& opt)
    __attribute__ ((__noreturn__));

  void invalid_option (const std::string& opt)
    __attribute__ ((__noreturn__));

  void invalid_option_val (const std::string& opt, const std::string& arg)
    __attribute__ ((__noreturn__));

  template<typename T>
  T convert_argument (const std::string& opt, const std::string& arg);
}

# include "libport/cli.hxx"

#endif // ! LIBPORT_CLI_HH
