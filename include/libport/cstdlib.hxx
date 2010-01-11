#ifndef LIBPORT_CSTDLIB_HXX
# define LIBPORT_CSTDLIB_HXX

/*---------.
| setenv.  |
`---------*/

# if defined _MSC_VER || defined __MINGW32__

extern "C"
{
  inline int
  setenv(const char* key, const char* value, int overwrite)
  {
    if (!overwrite && getenv(key))
      return 0;
#  if defined __MINGW32__ // MinGW has no _putenv_s
    return _putenv((std::string(key) + "=" + value).c_str());
#  else
    return _putenv_s(key, value);
#  endif
  }
}

# endif

/*----------.
| xgetenv.  |
`----------*/

extern "C"
{
  // It makes sense for this function to live in the C world, but it
  // is also put there so that our test suite can load libport.so and
  // look for "xgetenv" in it without worrying about mangling.
  inline const char*
  xgetenv(const char* c, const char* deflt)
  {
    const char* res = getenv(c);
    return res ? res : deflt;
  }
}

namespace libport
{
  inline std::string
  xgetenv(const char* c, const std::string& deflt)
  {
    const char* res = getenv(c);
    return res ? res : deflt;
  }
}

#endif
