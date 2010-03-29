/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_WINDOWS_HH
# define LIBPORT_WINDOWS_HH

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

#  include <libport/system-warning-push.hh>
#   include <io.h>
#   include <process.h>
#   include <windows.h>
#  include <libport/system-warning-pop.hh>

/* If we defined _WINSOCKAPI_ to prevent WinSock1 stuff to be imported,
 * restore the situation since the user might really want to import WinSock.h
 * later. */
#  ifdef LIBPORT_DEFINED_WINSOCKAPI_
#   undef _WINSOCKAPI_
#   undef LIBPORT_DEFINED_WINSOCKAPI_
#  endif

# endif // defined WIN32 || defined LIBPORT_WIN32

#endif // !LIBPORT_WINDOWS_HH
