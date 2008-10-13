#ifndef LIBPORT_CLI_HH
# define LIBPORT_CLI_HH

# include <string>
# include <libport/compiler.hh>
# include <libport/export.hh>

// Hopefully some day we will use Boost.ProgramOptions and get rid of
// this junk.  Yet, better share the junk than duplicate it.

namespace libport
{
  LIBPORT_API ATTRIBUTE_NORETURN
  void usage_error(const std::string& opt, const std::string& err);

  LIBPORT_API ATTRIBUTE_NORETURN
  void required_argument(const std::string& opt);

  LIBPORT_API ATTRIBUTE_NORETURN
  void missing_argument(const std::string& opt);

  LIBPORT_API ATTRIBUTE_NORETURN
  void invalid_option(const std::string& opt);

  LIBPORT_API ATTRIBUTE_NORETURN
  void invalid_option_val(const std::string& opt, const std::string& arg);

  /// Can be used on with T = const char* to simply require that \a arg
  /// is nonnull.
  template<typename T>
  T convert_argument(const std::string& opt, const std::string& arg);

  /// Convert the value contained in \a envvar to a \a T.
  /// \precondition getenv(envvar).
  template<typename T>
  T convert_envvar(const std::string& envvar);
}

# include <libport/cli.hxx>

#endif // !LIBPORT_CLI_HH
