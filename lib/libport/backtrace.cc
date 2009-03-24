#include <cstdlib>

#include <libport/backtrace.hh>
#include <libport/config.h>

#ifdef WIN32
# include <windows.h>
# include <winbase.h>
# include <cassert>

typedef USHORT (WINAPI *CSBT)(ULONG, ULONG, PVOID*, PULONG);

namespace libport
{
  std::vector<backtrace_type>
  backtrace()
  {
    void* array[63];
    HMODULE hdl = GetModuleHandle(TEXT("kernel32.dll"));
    assert(hdl);
    CSBT csbt = (CSBT) GetProcAddress(hdl,
				      "RtlCaptureStackBackTrace");
    assert(csbt);
    unsigned int frames = csbt(0, 63, array, 0);
    std::vector<backtrace_type> res;
    for (unsigned int i = 0; i < frames; ++i)
      res.push_back((char*) array[i]);
    return res;
  }
}

#else /* !WIN32 */

# if LIBPORT_HAVE_EXECINFO_H
#  include <execinfo.h>
#  include <cstdlib>

namespace libport
{
  std::vector<backtrace_type>
  backtrace()
  {
    enum { size = 128 };
    void* callstack[size];
    size_t frames = ::backtrace(callstack, size);
    char** strs = backtrace_symbols(callstack, frames);

    std::vector<backtrace_type> res(frames, 0);
    for (size_t i = 0; i < frames; ++i)
      res[i] = strs[i];

    free(strs);
    return res;
  }
}

# else // ! LIBPORT_HAVE_EXECINFO_H

namespace libport
{
  backtrace_type
  backtrace()
  {
    std::vector<backtrace_type> res;
    res.push_back("(no avaible backtrace)");
    return res;
  }
}

# endif

#endif /* !WIN32 */
