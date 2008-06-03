#ifndef LIBPORT_WINDOWS_HH
# define LIBPORT_WINDOWS_HH

# include <boost/cstdint.hpp>
# include <libport/detect-win32.h>

# if defined WIN32 || defined LIBPORT_WIN32

/* We don't want to include WinSock1 stuff that are included by Windows.h and
 * this is the official workaround used by Windows' headers. We're doing this
 * because WinSock1 stuff conflict with WinSock2 and this enables us to
 * include this header or libport/network.h in any order we want. */
#  ifndef _WINSOCKAPI_
#   define _WINSOCKAPI_
#   define LIBPORT_DEFINED_WINSOCKAPI_
#  endif

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

#  include <Windows.h>

/* If we defined _WINSOCKAPI_ to prevent WinSock1 stuff to be imported,
 * restore the situation since the user might really want to import WinSock.h
 * later. */
#  ifdef LIBPORT_DEFINED_WINSOCKAPI_
#   undef _WINSOCKAPI_
#   undef LIBPORT_DEFINED_WINSOCKAPI_
#  endif

// Based on the value I have on my G4 -- Akim.
typedef boost::uint32_t useconds_t;

// Some libraries define usleep as a macro. In this case, do not redefine
// it.
# ifndef usleep
inline
int
usleep (useconds_t microseconds)
{
  Sleep((microseconds + 999) / 1000);
  return 0;
}
# endif

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


# endif // !WIN32

#endif // !LIBPORT_WINDOWS_HH
