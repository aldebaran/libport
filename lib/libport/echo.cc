/*
 * Copyright (C) 2010, 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <algorithm>
#include <iostream>
#include <libport/unistd.h>
#include <libport/echo.hh>

namespace libport
{
  EchoPrologue::EchoPrologue(const std::string& file, unsigned line,
                             const std::string& function)
    : file_()
    , line_(line)
    , function_(function)
  {
    if (file.size() < length)
      file_ = file;
    else
    {
      file_ = "...";
      file_ += file.substr(std::max<unsigned>(0u, file.size() - length));
    }
  }

  std::ostream&
  EchoPrologue::dump(std::ostream& o) const
  {
    return o << (colored() ? "\x1b\x5b\x33\x33\x6d" : "")
             << file_ << ": "
             << line_ << ": "
             << function_ << ": "
             << (colored() ? "\x1b\x5b\x6d" : "");
  }

  bool
  EchoPrologue::colored()
  {
    // Hard coded hypothesis: we dump on stderr.
    static bool res = !getenv("BUILDFARM") && isatty(2);
    return res;
  }

}
