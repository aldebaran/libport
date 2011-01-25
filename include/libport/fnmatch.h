/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_FNMATCH
# define LIBPORT_FNMATCH

# include <libport/export.hh>
# include <libport/iostream>

# ifdef _MSC_VER
#  define FNM_NOMATCH 1
# endif

namespace libport
{

#ifdef _MSC_VER
  LIBPORT_API
#endif
  int
  fnmatch(const char* pattern, const char* string);

  int
  fnmatch(const std::string& pattern, const std::string& string);
}

#include <libport/fnmatch.hxx>

#endif // !LIBPORT_FNMATCH

// Local Variables:
// mode: C++
// End:
