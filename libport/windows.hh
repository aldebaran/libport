#ifndef LIBPORT_WINDOWS_HH
# define LIBPORT_WINDOWS_HH

# include "detect_win32.h"

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
typedef uint32_t useconds_t;

inline
int
usleep (useconds_t microseconds)
{
  Sleep(microseconds < 1000 ? 1 : microseconds / 1000);
  return 0;
}

inline
unsigned int
sleep(unsigned int seconds)
{
  Sleep(seconds * 1000);
  return 0;
}


// Define POSIX types if compiling with Visual C++.
#  ifdef _MSC_VER
// XXX: Maybe this ought to be in a new file, say, stdint.h?
typedef UINT8 uint8_t;
typedef INT8 int8_t;
typedef UINT16 uint16_t;
typedef INT16 int16_t;
typedef UINT32 uint32_t;
typedef INT32 int32_t;
typedef UINT64 uint64_t;
typedef INT64 int64_t;
#  endif

# endif // !WIN32

#endif // !LIBPORT_WINDOWS_HH
