/// \file libport/unistd.cc

#include <cerrno>
#include <iostream>

#include <libport/config.h>
#include <libport/program-name.hh>
#include <libport/sysexits.hh>
#include <libport/unistd.h>

namespace libport
{
  void
  exec(std::vector<const char*> args)
  {
    args.push_back(0);
    char* const* argv = const_cast<char* const*>(&args[0]);
    execv(argv[0], argv);

    std::cerr << libport::program_name()
              << ": failed to invoke " << args[0]
              << ": " << strerror(errno)
              << std::endl
              << libport::exit(EX_OSFILE);
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
