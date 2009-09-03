/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
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
