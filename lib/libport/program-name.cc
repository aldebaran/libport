/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cassert>
#include <libport/containers.hh>
#include <libport/program-name.hh>

namespace libport
{
  static std::string program_name_;
  static cli_args_type program_arguments_;
  static bool program_initialized_;

  void program_initialize(int argc, const char** argv)
  {
    aver(argc >= 1);
    program_initialize(cli_args_type(argv, argv + argc));
  }

  void program_initialize(int argc, char** argv)
  {
    program_initialize(argc, const_cast<const char**>(argv));
  }

  void program_initialize(const cli_args_type& args)
  {
    if (program_initialized_)
      return;
    aver(args.size() >= 1);
    program_name_ = args[0];
    program_arguments_.insert(program_arguments_.end(),
                              ++args.begin(), args.end());
    program_initialized_ = true;
  }

  void program_initialize(const std::string& program_name)
  {
    cli_args_type args;
    args << program_name;
    program_initialize(args);
  }

  const std::string& program_name()
  {
    aver(program_initialized_);
    return program_name_;
  }

  const cli_args_type& program_arguments()
  {
    aver(program_initialized_);
    return program_arguments_;
  }

}
