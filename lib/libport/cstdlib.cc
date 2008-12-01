#if defined WIN32

# include <string>

# include <libport/cstdlib>

extern "C"
{

  int
  setenv(const char* key, const char* value, int /* overwrite */)
  {
    // The windows version is a joke.
    return _putenv((std::string(key) + "=" + value).c_str());
  }

  int
  unsetenv(const char* key)
  {
    // The windows version is a BIG joke.
    // It looks like, to unset an environment variable under windows,
    // emptying it is sufficient. Which is pretty lame, since we thus
    // can't have a defined yet empty variable.
    return _putenv((std::string(key) + "=").c_str());
  }

}

#endif
