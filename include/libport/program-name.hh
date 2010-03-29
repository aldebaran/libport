/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_PROGRAM_NAME_HH
# define LIBPORT_PROGRAM_NAME_HH

# include <string>
# include <vector>

# include <libport/export.hh>

namespace libport
{
  /// Command line arguments.
  typedef std::vector<std::string> cli_args_type;

  // Initialize libport/program-name. One and only one of these
  // function must be called at the beginning of the main function.
  // Calling it twice is an error.
  LIBPORT_API void program_initialize(int argc, const char** argv);
  LIBPORT_API void program_initialize(int argc, char** argv);
  LIBPORT_API void program_initialize(const cli_args_type& args);
  LIBPORT_API void program_initialize(const std::string& program_name);
  // Retreive the program name (argv[0])
  LIBPORT_API const std::string& program_name();
  // Retreive the arguments
  LIBPORT_API const cli_args_type& program_arguments();
}

#endif // !LIBPORT_PROGRAM_NAME_HH
