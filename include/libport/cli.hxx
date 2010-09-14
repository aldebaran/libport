/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cerrno>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <libport/lexical-cast.hh>
#include <boost/utility/value_init.hpp>

#include <libport/cli.hh>
#include <libport/cstdio>
#include <libport/cstdlib>

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
  convert_argument<const char*>(const std::string& opt, const char* arg)
  {
    if (!arg)
      required_argument(opt);
    return arg;
  }


  template<typename T>
  T
  convert_argument(const std::string& opt, const std::string& arg)
  {
    return convert_argument<T>(opt, arg.c_str());
  }



  template<typename T>
  T
  convert_argument(const cli_args_type& args, unsigned i)
  {
    return convert_argument<T>(args[i],
                               i + 1 < args.size() ? args[i + 1].c_str() : 0);
  }

  template<> inline
  const std::string&
  convert_argument<const std::string&>(const cli_args_type& args, unsigned i)
  {
    if (! (i + 1 < args.size()))
      required_argument(args[i]);
    return args[i+1];
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
    if (i.fail())
      throw(std::runtime_error("cannot open `" + s + "' for reading: "
                               + strerror(errno)));
    // FIXME: The call to boost::get() below can be omitted when Boost 1.36.0
    // or later is available, by initializing res with "initialized_value".
    boost::value_initialized<T> res;
    i >> boost::get(res);
    if (i.fail())
      throw(std::runtime_error("cannot read expected contents from `"
                               + s + "': " + strerror(errno)));
#if 0
    // I don't know how to skip the spaces easily, so don't do that.
    if (!i.eof())
      throw(std::runtime_error("garbage after contents in `" + s + "'"));
#endif
    return res;
  }

}
