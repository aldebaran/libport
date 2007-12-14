#include <iostream>

#include "libport/cli.hh"
#include "libport/sysexits.hh"

extern const char* program_name;

namespace libport
{
  void
  usage_error (const std::string& opt, const std::string& err)
  {
    std::cerr << program_name << ": " << opt << ": " << err << std::endl;
    exit (EX_USAGE);
  }

  void
  required_argument (const std::string& opt)
  {
    usage_error (opt, "requires an argument");
  }

  void
  missing_argument (const std::string& opt)
  {
    usage_error (opt, "missing argument");
  }

  void
  invalid_option (const std::string& opt)
  {
    usage_error (opt, "invalid option");
  }

  void
  invalid_option_val (const std::string& opt, const std::string& arg)
  {
    usage_error (opt, "invalid argument: " + arg);
  }

}
