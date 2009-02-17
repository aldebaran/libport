#include <cassert>

#include <libport/program-name.hh>

namespace libport
{
  static std::string program_name_;
  static cli_args_type program_arguments_;
  static bool program_initialized_;

  LIBPORT_API void program_initialize(int argc, char** argv)
  {
    assert(argc >= 1);
    cli_args_type args;
    for (int i = 0; i < argc; ++i)
      args.push_back(argv[i]);
    program_initialize(args);
  }

  LIBPORT_API void program_initialize(cli_args_type args)
  {
    assert(!program_initialized_);
    assert(args.size() >= 1);
    program_name_ = args[0];
    for (cli_args_type::iterator it = ++args.begin();
         it != args.end();
         ++it)
      program_arguments_.push_back(*it);
    program_initialized_ = true;
  }

  void program_initialize(const std::string& program_name)
  {
    cli_args_type args;
    args.push_back(program_name);
    program_initialize(args);
  }

  LIBPORT_API const std::string& program_name()
  {
    assert(program_initialized_);
    return program_name_;
  }

  LIBPORT_API const cli_args_type& program_arguments()
  {
    assert(program_initialized_);
    return program_arguments_;
  }

}
