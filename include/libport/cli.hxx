#include <libport/cstdio>
#include <cerrno>
#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include <libport/cli.hh>
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
                << s << "' for reading: " << strerror(errno)
                << std::endl;
    T res;
    i >> res;
    if (i.bad())
      std::cerr << program_name << ": cannot read an integer from `"
                << s << "': " << strerror(errno)
                << std::endl;
    if (!i.eof())
      std::cerr << program_name << ": garbage after the integer in `"
                << s << "': " << strerror(errno)
                << std::endl;
    return res;
  }

}
