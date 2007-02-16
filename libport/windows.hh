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
   and std::max.  We might need some magic to bind _cpp_min and
   _cpp_max to min and max eventually.  See
   <http://raduking.homeip.net/raduking/forumwin/index.php?showtopic=270>.
   */

#  define NOMINMAX
#   include <Windows.h>
#  undef NOMINMAX

/* If we defined _WINSOCKAPI_ to prevent WinSock1 stuff to be imported,
 * restore the situation since the user might really want to import WinSock.h
 * later. */
#  ifdef LIBPORT_DEFINED_WINSOCKAPI_
#   undef _WINSOCKAPI_
#   undef LIBPORT_DEFINED_WINSOCKAPI_
#  endif

#  define usleep(a) Sleep((a) < 1000 ? 1 : (a) / 1000)
#  define sleep(a) Sleep((a) * 1000)

# endif // !WIN32

#endif // !LIBPORT_WINDOWS_HH
