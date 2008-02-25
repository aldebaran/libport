/**
 ** \file libport/string.cc
 ** \brief string: implements file libport/string.hh
 */

#include <boost/lexical_cast.hpp>

#include "libport/string.hh"

#include "libport/_errlist.hh"

namespace libport
{

  const char*
  strerror(int errnum)
  {
#ifndef WIN32
    return ::strerror(errnum);
#else
    for (unsigned i = 0; _sys_errlist[i].code >= 0; ++i)
      if (_sys_errlist[i].code == errnum)
        return _sys_errlist[i].msg;

    try
    {
      std::string msg = "Unknown error code : "
        + boost::lexical_cast<std::string>(errnum);
      return msg.c_str();
    }
    catch (boost::bad_lexical_cast &msg)
    {
      return "Unknown error code";
    }
#endif
  }

}
