/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef FNMATCH_HXX
# define FNMATCH_HXX

# include <libport/fnmatch.h>

# ifndef _MSC_VER
#  include <fnmatch.h>
# endif

namespace libport
{

# ifndef _MSC_VER
  /// If os is not Windows, bounce on bsd fnmatch.  Use no specific
  /// flags, and return boolean instead of integer.
  inline int
  fnmatch (const char* pattern, const char* string)
  {
    return ::fnmatch(pattern, string, 0);
  }
# endif

  inline int
  fnmatch(const std::string& pattern, const std::string& string)
  {
    return ::libport::fnmatch(pattern.c_str(), string.c_str());
  }

}

#endif
