#include <iostream>

#include <libport/cli.hh>
#include <libport/program-name.hh>

namespace libport
{

  void
  usage_error(const std::string& opt, const std::string& err)
  {
    throw(std::runtime_error
          (opt + ": " + err + "\n"
           + "Try `" + program_name() + " --help' for more information."));
  }

  void
  required_argument(const std::string& opt)
  {
    usage_error(opt, "requires an argument");
  }

  void
  missing_argument(const std::string& opt)
  {
    usage_error(opt, "missing argument");
  }

  void
  invalid_option(const std::string& opt)
  {
    usage_error(opt, "invalid option");
  }

  void
  invalid_option_val(const std::string& opt, const std::string& arg)
  {
    usage_error(opt, "invalid argument: " + arg);
  }

}
