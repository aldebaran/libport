#include <cstdlib>

#include <libport/backtrace.hh>
#include <libport/config.h>
#include <libport/containers.hh>

#ifdef WIN32
# include <windows.h>
# include <winbase.h>
# include <cassert>

typedef USHORT (WINAPI *CSBT)(ULONG, ULONG, PVOID*, PULONG);

namespace libport
{
  backtrace&
  backtrace(backtrace& res)
  {
    void* array[63];
    HMODULE hdl = GetModuleHandle(TEXT("kernel32.dll"));
    assert(hdl);
    CSBT csbt = (CSBT) GetProcAddress(hdl,
				      "RtlCaptureStackBackTrace");
    assert(csbt);
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
# include <cstdlib>

namespace libport
{
  backtrace&
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
  backtrace&
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
}
