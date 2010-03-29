/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstdlib>
#include <iostream>

#include <libport/backtrace.hh>
#include <libport/config.h>
#include <libport/containers.hh>

#ifdef WIN32
# include <windows.h>
# include <winbase.h>
# include <libport/cassert>

typedef USHORT (WINAPI *CSBT)(ULONG, ULONG, PVOID*, PULONG);

namespace libport
{
  backtrace_type&
  backtrace(backtrace_type& res)
  {
    void* array[63];
    HMODULE hdl = GetModuleHandle(TEXT("kernel32.dll"));
    aver(hdl);
    CSBT csbt = (CSBT) GetProcAddress(hdl,
				      "RtlCaptureStackBackTrace");
    aver(csbt);
    size_t frames = csbt(0, 63, array, 0);
    res.clear();
    res.reserve(frames);
    for (size_t i = 0; i < frames; ++i)
      res << (char*) array[i];
    return res;
  }
}

#elif LIBPORT_HAVE_EXECINFO_H
# include <execinfo.h>
# include <libport/cstdlib>

namespace libport
{
  backtrace_type&
  backtrace(backtrace_type& res)
  {
    enum { size = 128 };
    void* callstack[size];
    size_t frames = ::backtrace(callstack, size);
    char** strs = backtrace_symbols(callstack, frames);

    res.clear();
    res.reserve(frames);
    for (size_t i = 0; i < frames; ++i)
      res << strs[i];
    // Free strs, and the strings in contains.
    free(strs);
    return res;
  }
}

# else // ! WIN32 && ! LIBPORT_HAVE_EXECINFO_H

namespace libport
{
  backtrace_type&
  backtrace(backtrace_type& res)
  {
    res.clear();
    res << "(backtrace not available)";
    return res;
  }
}

#endif

namespace libport
{
  backtrace_type
  backtrace()
  {
    backtrace_type res;
    return backtrace(res);
  }


  Backtrace::Backtrace()
    : backtrace_()
  {
    backtrace(backtrace_);
  }

  const backtrace_type&
  Backtrace::backtrace_get() const
  {
    return backtrace_;
  }

  std::ostream&
  Backtrace::dump(std::ostream& o) const
  {
    foreach(const std::string& s, backtrace_)
      o << s << std::endl;
    return o;
  }

}
