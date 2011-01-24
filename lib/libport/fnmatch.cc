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
 ** \file libport/cstdio.cc
 ** \brief Implements file libport/cstdio.
 */

#include <libport/fnmatch.h>

#ifdef WIN32
# include <Shlwapi.h>
#endif

namespace libport
{
#ifdef WIN32
  /// On Windows use PathMatchSpec.
  int
  fnmatch(const char* pattern, const char* string)
  {
    return ::PathMatchSpec(string, pattern);
  }
#endif
}
