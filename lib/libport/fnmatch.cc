/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/fnmatch.cc
 ** \brief Implements file libport/fnmatch.h.
 */

#include <libport/fnmatch.h>

#ifdef _MSC_VER
# include <Shlwapi.h>
# pragma comment(lib, "shlwapi.lib")
#endif

namespace libport
{
#ifdef _MSC_VER
  /// On Windows use PathMatchSpec.
  int
  fnmatch(const char* pattern, const char* string)
  {
    return ::PathMatchSpec(string, pattern) ? 0 : FNM_NOMATCH;
  }
#endif
}
