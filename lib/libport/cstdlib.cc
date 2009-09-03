/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#include <libport/detect-win32.h>
#include <string>
#include <libport/cstdlib>

#if defined _MSC_VER || defined __MINGW32__

extern "C"
{
  int
  setenv(const char* key, const char* value, int overwrite)
  {
    if (!overwrite && getenv(key))
      return 0;
#if defined __MINGW32__ // MinGW has no _putenv_s
    return _putenv((std::string(key) + "=" + value).c_str());
#else
    return _putenv_s(key, value);
#endif
  }
}

#endif

#ifdef _MSC_VER

extern "C"
{
  long long
  strtoll(const char* nptr, char** endptr, int base)
  {
    return _strtoi64(nptr, endptr, base);
  }
}
#endif


#if defined _MSC_VER || defined __MINGW32__

extern "C"
{
  int
  unsetenv(const char* key)
  {
    // Windows refuses 0 as a value.  But passing "" actually unsets
    // the envvar.
    return setenv(key, "", 1);
  }
}

#endif

/*----------.
| xgetenv.  |
`----------*/

extern "C"
{
  // It makes sense for this function to live in the C world, but it
  // is also put there so that our test suite can load libport.so and
  // look for "xgetenv" in it without worrying about mangling.
  const char*
  xgetenv(const char* c, const char* deflt)
  {
    const char* res = getenv(c);
    return res ? res : deflt;
  }
}

namespace libport
{
  std::string
  xgetenv(const char* c, const std::string& deflt)
  {
    const char* res = getenv(c);
    return res ? res : deflt;
  }
}
