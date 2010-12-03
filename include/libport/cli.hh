/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_CLI_HH
# define LIBPORT_CLI_HH

# include <string>
# include <libport/compiler.hh>
# include <libport/export.hh>
# include <libport/program-name.hh>

// Hopefully some day we will use Boost.ProgramOptions and get rid of
// this junk.  Better share the junk than duplicate it.

namespace libport
{
  LIBPORT_API ATTRIBUTE_NORETURN
  /// \throw std::runtime_error
  void usage_error(const std::string& err);

  LIBPORT_API ATTRIBUTE_NORETURN
  /// An option \a opt was called without its required argument.
  /// \throw std::runtime_error
  void required_argument(const std::string& opt);

  LIBPORT_API ATTRIBUTE_NORETURN
  /// An invalid option \a opt was invoked.
  /// \throw std::runtime_error
  void invalid_option(const std::string& opt);

  LIBPORT_API ATTRIBUTE_NORETURN
  /// An option \a opt was given an improper value \a arg.
  /// \throw std::runtime_error
  void invalid_option_val(const std::string& opt, const std::string& arg);

  /// Can be used on with T = const char* to simply require that \a arg
  /// is nonnull.
  template<typename T>
  T convert_argument(const std::string& opt, const char* arg);

  /// Same, but in C++.
  template<typename T>
  T convert_argument(const std::string& opt, const std::string& arg);

  /// Same but in C++.
  /// \param args     argv/argc as a C++ object.
  /// \param optind   index of the option (argument is expected at optind+1).
  template<typename T>
  T convert_argument(const cli_args_type& args, unsigned optind);

  /// Convert the value contained in \a envvar to a \a T.
  /// \precondition getenv(envvar).
  template<typename T>
  T convert_envvar(const std::string& envvar);

  /// Read the file \a s and return its contents as \a T.
  /// It must contain a T, and nothing else.  Based on the operator>>.
  template <typename T>
  T
  file_contents_get(const std::string& s);
}

# include <libport/cli.hxx>

#endif // !LIBPORT_CLI_HH
