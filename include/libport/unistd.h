#ifndef LIBPORT_UNISTD_H
# define LIBPORT_UNISTD_H

# include <libport/detect-win32.h>
# include <libport/export.hh>
# include <libport/config.h>

// This is traditional Unix file.
# ifdef LIBPORT_HAVE_UNISTD_H
#  include <unistd.h>
# endif

// Several functions (e.g., close, read, write) are defined in io.h.
// http://msdn2.microsoft.com/en-us/library/ms811896(d=printer).aspx#ucmgch09_topic7.
# if defined WIN32 || defined LIBPORT_WIN32
/* We don't want the min and max macros that conflict with std::min
 * and std::max.  We might need some magic to bind _cpp_min and
 * _cpp_max to min and max eventually.  See
 * <http://raduking.homeip.net/raduking/forumwin/index.php?showtopic=270>.
 *
 * This must be done conditionnaly because MinGW already defines NOMINMAX in
 * some headers.  */
#  ifndef NOMINMAX
#   define NOMINMAX 1
#  endif

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
    return closesocket(static_cast<SOCKET>(socket));
# else
    return close(socket);
# endif
  }

}


/*--------------.
| getpagesize.  |
`--------------*/

# if defined WIN32
extern "C"
{
  inline
  int
  getpagesize()
  {
    // FIXME: find the equivalent call.
    return 4096;
  }
}
# endif

/*---------.
| getpid.  |
`---------*/

# if defined WIN32
#  include <process.h>
# endif


/*---------.
| isatty.  |
`---------*/


# if defined _MSC_VER
#  include <libport/fcntl.h>
#  define STDIN_FILENO  fileno(stdin)
#  define STDOUT_FILENO fileno(stdout)
#  define STDERR_FILENO fileno(stderr)
# endif



/*-------.
| pipe.  |
`-------*/

# if defined WIN32
extern "C"
{
  LIBPORT_API int pipe(int pipefd[2]);
}
# endif


/*----------.
| ssize_t.  |
`----------*/

# if defined _MSC_VER
extern "C"
{
  typedef int ssize_t;
}
# endif


/*--------.
| sleep.  |
`--------*/

# if defined WIN32
extern "C"
{
  inline
  unsigned int
  sleep(unsigned int seconds)
  {
    Sleep(seconds * 1000);
    // Under Unix systems, sleep returns the number of second left to
    // sleep if interrupted by a signal. The WIN32 Sleep always sleeps
    // the requested time, thus 0 is the right answer.
    return 0;
  }
}
# endif


/*---------.
| usleep.  |
`---------*/

# if defined WIN32
#  include <boost/cstdint.hpp>
extern "C"
{
  // Based on the value I have on my G4 -- Akim.
  typedef boost::uint32_t useconds_t;
}

// Some libraries define usleep as a macro. In this case, do not redefine
// it.
#  ifndef usleep
extern "C"
{
  inline
  int
  usleep(useconds_t microseconds)
  {
    Sleep((microseconds + 999) / 1000);
    return 0;
  }
}
#  endif // !usleep
# endif

#endif // !LIBPORT_UNISTD_H
