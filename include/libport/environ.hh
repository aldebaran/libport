#ifndef LIBPORT_ENVIRON_HH
# define LIBPORT_ENVIRON_HH

# include <libport/detect-win32.h>

# ifdef WIN32
#  include <stdlib.h>
#  include <string>

int setenv(const char* key, const char* value, int /* overwrite */)
{
  // The windows version is a joke.
  return _putenv((std::string(key) + "=" + value).c_str());
}

int unsetenv(const char* key)
{
  // The windows version is a BIG joke.
  // It looks like, to unset an environment variable under windows,
  // emptying it is sufficient. Which is pretty lame, since we thus
  // can't have a defined yet empty variable.
  return _putenv((std::string(key) + "=").c_str());
}
# endif

#endif
