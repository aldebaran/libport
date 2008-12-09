# include <libport/detect-win32.h>
#if defined WIN32

# include <string>
# include <libport/cstdlib>

extern "C"
{

  int
  setenv(const char* key, const char* value, int overwrite)
  {
    if (!overwrite && getenv(key))
      return 0;
    return _putenv_s(key, value);
  }

  int
  unsetenv(const char* key)
  {
    // Windows refuses 0 as a value.  But passing "" actually unsets
    // the envvar.
    return setenv(key, "", 1);
  }

}

#endif
