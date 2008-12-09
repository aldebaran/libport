#ifndef LIBPORT_UNISTD_H
# define LIBPORT_UNISTD_H

/*-----------------------------------------------.
| FIXME: #define is evil, use wrappers instead.  |
`-----------------------------------------------*/

# include <libport/detect-win32.h>
# include <libport/windows.hh> // Get sleep wrapper

# include <libport/config.h>

// This is traditional Unix file.
# ifdef LIBPORT_HAVE_UNISTD_H
#  include <unistd.h>
# endif

// OSX does not have O_LARGEFILE.  No information was found whether
// some equivalent flag is needed or not.  Other projects simply do as
// follows.
# ifndef O_LARGEFILE
#  define O_LARGEFILE 0
# endif

// Several functions (e.g., close, read, write) are defined in io.h.
// http://msdn2.microsoft.com/en-us/library/ms811896(d=printer).aspx#ucmgch09_topic7.
# if defined WIN32 || defined LIBPORT_WIN32
#  include <io.h>
# endif

/*---------.
| access.  |
`---------*/

# ifdef WIN32
// Windows does not define access() or the corresponding constants.
// http://msdn.microsoft.com/en-us/library/1w06ktdy(VS.80).aspx
#  define F_OK 0
#  define W_OK 2
#  define R_OK 4
// #  define access _access
# endif

/*--------.
| chdir.  |
`--------*/

# ifdef WIN32
#  include <direct.h>
#  define chdir _chdir
# endif

/*--------------.
| closesocket.  |
`--------------*/

# include <libport/sys/socket.h>

namespace libport
{

  inline
  int
  closeSocket(int socket)
  {
# if defined WIN32
    return closesocket(socket);
# else
    return close(socket);
# endif
  }

}

/*---------.
| getcwd.  |
`---------*/

# if !defined LIBPORT_HAVE_GETCWD
#  if defined LIBPORT_HAVE__GETCWD
#   define getcwd _getcwd
#  elif defined LIBPORT_URBI_ENV_AIBO
    // Will be defined in libport/unistd.cc.
#  else
#   error I need either getcwd() or _getcwd()
#  endif
# endif

/*-------.
| getpid |
`-------*/

# if defined WIN32
#  include <process.h>
typedef int pid_t;
inline pid_t getpid()
{
  return _getpid();
}
# endif

/*---------.
| unlink.  |
`---------*/

# if defined WIN32
inline
int unlink(const char* pathname)
{
  return _unlink(pathname);
}
# endif

#endif // !LIBPORT_UNISTD_H
