#include <libport/backtrace.hh>
#include <libport/config.h>

#if LIBPORT_HAVE_EXECINFO_H
# include <execinfo.h>

namespace libport
{
  std::vector<const char*>
  backtrace()
  {
    enum { size = 128 };
    void* callstack[size];
    size_t frames = ::backtrace(callstack, size);
    char** strs = backtrace_symbols(callstack, frames);

    std::vector<const char*> res(frames, 0);
    for (size_t i = 0; i < frames; ++i)
      res[i] = strs[i];

    free(strs);
    return res;
  }
}

#else // ! LIBPORT_HAVE_EXECINFO_H

namespace libport
{
  std::vector<const char*>
  backtrace()
  {
    std::vector<const char*> res;
    res.push_back("(no avaible backtrace)");
  }
}

#endif

