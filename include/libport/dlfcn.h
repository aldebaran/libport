/*
 * Copyright (C) 2010, 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/** Portable dlopen utilities. -*- c++ -*-
 *
 *  Keep this file header only as it's used in relocatable binaries.
 */

#ifndef LIBPORT_DLFCN_H
# define LIBPORT_DLFCN_H

# include <libport/windows.hh>

/*---------------------------------.
| dlopen and dlsym under windows.  |
`---------------------------------*/

# ifdef WIN32
#  define RTLD_LAZY 0
#  define RTLD_NOW 0
#  define RTLD_GLOBAL 0

typedef HMODULE RTLD_HANDLE;

static inline RTLD_HANDLE
dlopen(const char* name, int)
{
  RTLD_HANDLE res = LoadLibrary(name);
  if (res)
  {
    char buf[BUFSIZ];
    GetModuleFileName(res, buf, sizeof buf - 1);
    buf[sizeof buf - 1] = 0;
  }
  return res;
}

static inline void*
dlsym(RTLD_HANDLE module, const char* name)
{
  return GetProcAddress(module, name);
}

static inline const char*
dlerror(DWORD err = GetLastError())
{
  static char buf[BUFSIZ];
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                0, err, 0,
                (LPTSTR)buf, sizeof buf,
                0);
  return buf;
}

# else
#  include <dlfcn.h>
typedef void* RTLD_HANDLE;
# endif

/*-----------------------------.
| Portable file names chunks.  |
`-----------------------------*/

namespace libport
{
  /// Extension of executable files, potential dot included.
  static const std::string ext_exe = ""
# ifdef WIN32
    ".exe"
# endif
    ;

  /// Extension of shared modules, potential dot included.
  static const std::string ext_module =
# if defined WIN32
  ".dll"
# else
  ".so"
# endif
  ;

  /// Extension of shared libraries, potential dot included.
  static const std::string ext_shlib =
# if defined WIN32
  ".dll"
# elif defined __APPLE__
  ".dylib"
# else
  ".so"
# endif
  ;

  /// Potential prefix to prepend to shared libraries names.
  static const std::string pre_shlib = ""
# ifndef WIN32
  "lib"
# endif
  ;
}

#endif
