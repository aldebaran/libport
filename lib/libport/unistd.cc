/// \file libport/unistd.cc

#include <libport/unistd.h>
#include <libport/config.h>


/*--------.
| chdir.  |
`--------*/

#ifdef WIN32
# if 0
extern "C"
{
  int
  chdir(const char* path)
  {
    return _chdir(path);
  }
}
# endif
#endif


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

/*---------.
| unlink.  |
`---------*/

#if defined WIN32
extern "C"
{
  int
  unlink(const char* p)
  {
    return _unlink(p);
  }
}
#endif
