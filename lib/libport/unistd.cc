/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/unistd.cc

#include <libport/cerrno>
#include <libport/cstring>
#include <iostream>
#include <stdexcept>

#include <libport/config.h>
#include <libport/containers.hh>
#include <libport/format.hh>
#include <libport/separate.hh>
#include <libport/unistd.h>

namespace libport
{

  void
  exec(std::vector<const char*> args, bool path_lookup)
  {
    args.push_back(0);
    char* const* argv = const_cast<char* const*>(&args[0]);
    if (path_lookup)
      execvp(argv[0], argv);
    else
      execv(argv[0], argv);

    throw std::runtime_error(format("failed to %s(%s): %s",
                                    path_lookup ? "execvp" : "execv",
                                    libport::separate(args, " "),
                                    strerror(errno)));
  }

  void
  exec(const std::vector<std::string>& args, bool path_lookup)
  {
    std::vector<const char*> as;
    as.reserve(args.size() + 2);
    foreach (const std::string& s, args)
      as << s.c_str();
    exec(as, path_lookup);
  }
}

/*-------.
| pipe.  |
`-------*/

#ifdef WIN32
# include <libport/cstdio> // BUFSIZ
# include <libport/fcntl.h> // O_BINARY
extern "C"
{
  int
  pipe(int pipefd[2])
  {
    // fds, memory to reserve, mode
    return _pipe(pipefd, BUFSIZ, _O_BINARY);
  }
}
#endif
