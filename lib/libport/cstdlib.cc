#if defined WIN32

# include <string>

# include <libport/cstdlib>

extern "C"
{

  int
  setenv(const char* key, const char* value, int overwrite)
  {
    if (!overwrite && getenv(name))
      return 0;
    return _putenv_s(key, value);
  }

  int
  unsetenv(const char* key)
  {
    return setenv(key, 0, 1);
  }

}

#endif
