/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */


// Needed to get rand_s. Don't take any chance, define it right away.
#if defined _MSC_VER && !defined _CRT_RAND_S
# define _CRT_RAND_S
#endif

#include <stdexcept>
#include <iostream>

#include <libport/backtrace.hh>
#include <libport/cassert>
#include <libport/cstdlib>
#include <libport/detect-win32.h>
#include <libport/exception.hh>
#include <libport/foreach.hh>
#include <libport/format.hh>
#include <libport/sys/wait.h>
#include <string>

# ifdef _MSC_VER
#  include <crtdbg.h> // _CrtDbgBreak
#  include <libport/windows.hh>
# endif

# if defined __APPLE__
#  include <crt_externs.h>
# endif

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

namespace libport
{
  const char*
  getenv(const char* name)
  {
    return ::getenv(name);
  }
}

#endif

namespace libport
{
  int rand()
  {
#if defined _MSC_VER
    unsigned int res;
    errno_t err = rand_s(&res);
    // FIXME: Actual error handling?
    assert_eq(err, 0);
    return res;
#else
    return ::rand();
#endif
  }
}

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

/*-------------.
| getenviron.  |
`-------------*/

namespace libport
{

  inline
  void
  env_add(environ_type& env, const char *var)
  {
    const char* eq = strchr(var, '=');
    env[std::string(var, eq - var)] = std::string(eq + 1);
  }

  inline
  environ_type
  env_build(char **env)
  {
    environ_type res;
    for (size_t i = 0; env[i]; ++i)
      env_add(res, env[i]);
    return res;
  }

  environ_type
  getenviron()
  {
    environ_type res;
# if defined __APPLE__
    res = env_build(*_NSGetEnviron());
# elif defined _MSC_VER || defined __MINGW32__
    char* env = GetEnvironmentStrings();
    for (char* cp = env; *cp; cp += strlen(cp) + 1)
      env_add(res, cp);
    FreeEnvironmentStrings(env);
# else
     res = env_build(environ);
# endif
    return res;
  }
}


/*--------.
| abort.  |
`--------*/

namespace libport
{
  bool abort_throw_ = false;
  void abort_throw(bool v)
  {
    abort_throw_ = v;
  }

  void
  abort(const std::string& msg)
  {
    if (abort_throw_)
      throw std::runtime_error(msg);
    else
    {
      std::cerr << msg << " (backtrace follows)" << std::endl;
      foreach(const std::string& str, libport::backtrace())
        std::cerr << "  " << str << std::endl;
# ifdef _MSC_VER
      if (getenv("_DEBUG_VCXX"))
        _CrtDbgBreak();
      else
# endif
        std::abort();
    }
  }

  void
  abort(const char* file, int line, const std::string& msg)
  {
    std::string err = libport::format("%s:%s: abort", file, line);
    if (!msg.empty())
      err += ": " + msg;
    abort(err);
  }


}

/*----------.
| xsystem.  |
`----------*/

namespace libport
{
  int
  system(const std::string& cmd)
  {
    int res = ::system(cmd.c_str());
    if (res == -1)
      errnoabort("system");
    return res;
  }

  void
  xsystem(const std::string& cmd)
  {
    int res = system(cmd);
    if (!WIFEXITED(res))
      throw libport::Exception("command was signaled");
    if (int rv = WEXITSTATUS(res))
      throw Exception(format("command failed: %s", rv));
  }
}
