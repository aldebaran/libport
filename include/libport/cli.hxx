#include <boost/lexical_cast.hpp>

#include "libport/cli.hh"

namespace libport
{

  template<typename T>
  T
  convert_argument (const std::string& opt, const char* arg)
  {
    if (!arg)
      required_argument (opt);

    try
    {
      return boost::lexical_cast<T>(arg);
    }
    catch (boost::bad_lexical_cast&)
    {
      invalid_option_val (opt, arg);
    }
  }

}

