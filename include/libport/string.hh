/**
 ** \file libport/string.hh
 ** \brief Provide a Windows version of strerror.
 **/

#ifndef LIBPORT_STRING_HH
# define LIBPORT_STRING_HH

# include <string.h>

namespace libport
{

  const char*
  strerror(int errnum);

}

#endif // !LIBPORT_STRING_HH
