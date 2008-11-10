#include <libport/cstdio>
#include <cerrno>
#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/utility/value_init.hpp>

#include <libport/cli.hh>
#include <libport/sysexits.hh>
#include <libport/program-name.hh>

namespace libport
{

  template<typename T>
  T
  convert_argument(const std::string& opt, const char* arg)
  {
    if (!arg)
      required_argument(opt);

    try
    {
      return boost::lexical_cast<T>(arg);
    }
    catch (boost::bad_lexical_cast&)
    {
      invalid_option_val(opt, arg);
    }
  }

  template<> inline
  const char *
  convert_argument<>(const std::string& opt, const char* arg)
  {
    if (!arg)
      required_argument(opt);
    return arg;
  }

  template<typename T>
  T
  convert_argument(const cli_args_type& args, unsigned i)
  {
    return convert_argument<T>(args[i],
                               i + 1 < args.size() ? args[i + 1].c_str() : 0);
  }

  template<typename T>
  T
  convert_envvar(const std::string& envvar)
  {
    return convert_argument<T>(std::string("$") + envvar,
                               getenv(envvar.c_str()));
  }


  template <typename T>
  T
  file_contents_get(const std::string& s)
  {
    std::ifstream i(s.c_str());
    if (i.bad())
      std::cerr << program_name << ": cannot open `"
                << s << "' for reading: " << strerror(errno) << std::endl
                << libport::exit(EX_NOINPUT);
    // FIXME: The call to boost::get() below can be omitted when Boost 1.36.0
    // or later is available, by initializing res with "initialized_value".
    boost::value_initialized<T> res;
    i >> boost::get(res);
    if (i.bad())
      std::cerr << program_name << ": cannot read expected contents from `"
                << s << "': " << strerror(errno) << std::endl
                << libport::exit(EX_DATAERR);

#if 0
    // I don't know how to skip the spaces easily, so don't do that.
    if (!i.eof())
      std::cerr << program_name << ": garbage after contents in `"
                << s << "'" << std::endl
                << libport::exit(1);
#endif
    return res;
  }

}
