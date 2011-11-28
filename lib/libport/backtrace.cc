/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
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
# include <dbghelp.h>
# include <libport/cassert>

# define win_aver(Cond)                                 \
  if (!Cond)                                            \
  {                                                     \
    DWORD error = GetLastError();                       \
    std::cerr << "Error " << error << std::endl;        \
    aver(Cond);                                         \
  }

# define LOAD_LIBRARY(Lib)                              \
  HMODULE Lib = GetModuleHandle(TEXT(#Lib ".dll"));     \
  if (!Lib)                                             \
    Lib = LoadLibrary(TEXT(#Lib ".dll"));               \
  win_aver(Lib)

# define LOAD_FUNCTION_PREFIX(Fun, Prefix, Lib)                 \
  Fun ## _t Fun = (Fun ## _t) GetProcAddress(Lib, Prefix #Fun); \
  win_aver(Fun)

# define LOAD_FUNCTION(Fun, Lib)                                \
  Fun ## _t Fun = (Fun ## _t) GetProcAddress(Lib, #Fun);        \
  win_aver(Fun)

namespace libport
{
  backtrace_type&
  backtrace(backtrace_type& res)
  {
    // Avoid looping when an assertion fail.
    static bool execute_backtrace = false;
    if (execute_backtrace)
    {
      res.clear();
      return res;
    }
    execute_backtrace = true;

    // Load function definitions from windows libraries.

    typedef HANDLE (WINAPI *GetCurrentProcess_t)(void);
    typedef USHORT (WINAPI *CaptureStackBackTrace_t)(
      ULONG, ULONG, PVOID*, PULONG);

    typedef DWORD (WINAPI *SymSetOptions_t)(DWORD);
    typedef BOOL (WINAPI *SymInitialize_t)(HANDLE, PCTSTR, BOOL);
    typedef BOOL (WINAPI *SymFromAddr_t)(
      HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO);
    typedef BOOL (WINAPI *SymGetLineFromAddr64_t)(
      HANDLE, DWORD64, PDWORD64, PIMAGEHLP_LINE64);

    LOAD_LIBRARY(kernel32);
    LOAD_FUNCTION(GetCurrentProcess, kernel32);
    LOAD_FUNCTION_PREFIX(CaptureStackBackTrace, "Rtl", kernel32);

    LOAD_LIBRARY(dbghelp);
    LOAD_FUNCTION(SymSetOptions, dbghelp);
    LOAD_FUNCTION(SymInitialize, dbghelp);
    LOAD_FUNCTION(SymFromAddr, dbghelp);
    LOAD_FUNCTION(SymGetLineFromAddr64, dbghelp);

    // The real code of the backtrace function start Here.

    void* array[63];
    HANDLE process;
    IMAGEHLP_LINE64* line;
    SYMBOL_INFO* symbol;
    DWORD64 displacement;

    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
    process = GetCurrentProcess();

    if (!SymInitialize(process, 0, true))
    {
      res.clear();
      execute_backtrace = false;
      return res;
    }

    size_t frames = CaptureStackBackTrace(0, 63, array, 0);

    // Symbol names are wrote after the structure's end, so we need to
    // allocate extra space for it.
    symbol = (SYMBOL_INFO *)
      calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(TCHAR), 1);

    // Initialized field of the structure which are used by SymFromAddr to
    // fill other fields.
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = 255;

    // Initialized the structure in which PDB informations would be added.
    line = (IMAGEHLP_LINE64 *) calloc(sizeof(IMAGEHLP_LINE64), 1);
    line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    // Clear the vector of frames.
    res.clear();
    res.reserve(frames);

    for (size_t i = 0; i < frames; ++i)
    {
      bool printable = false;
      std::ostringstream ostr;

      // Call after all previous information have been stored because
      // functions could sahre buffers.
      if (SymGetLineFromAddr64(process, (size_t) array[i], &displacement, line))
      {
        printable = true;
        ostr << line->FileName << ":" << std::dec << line->LineNumber << " ";
      }

      // see also SymGetSymFromAddr64 and SymGetLineFromAddr64
      if (SymFromAddr(process, (size_t) array[i], &displacement, symbol))
      {
        printable = true;
        ostr << symbol->Name << std::hex << " +0x" << displacement
             << "[0x" << symbol->Address << "]";
      }
      else
        ostr << "???";

      if (printable)
        res << ostr.str();
    }

    free(line);
    free(symbol);
    execute_backtrace = false;
    return res;
  }
}

# undef LOAD_FUNCTION
# undef LOAD_FUNCTION_PREFIX
# undef LOAD_LIBRARY

# undef win_aver

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
