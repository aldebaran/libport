/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \brief Turn off many warnings before including system headers
///        such as Boost headers.  Be sure to reenable warnings
///        afterards using libport/system-warning-pop.hh.

#ifdef _MSC_VER
# pragma warning( push )
#endif

#include <libport/warning-push.hh>

// vcxx8/VC/PlatformSDK/Include/WSPiApi.h:893:
// warning C4191: 'type cast' : unsafe conversion from 'int
// (__stdcall *)(const char *,const char *,const addrinfo *,addrinfo **)'
// to 'FARPROC'
// Calling this function through the result pointer may cause your
// program to fail.

// boost/asio/detail/socket_ops.hpp:1274:
// warning C4242: '=' : conversion from 'int' to 'u_short',
//                possible loss of data

// boost/asio/detail/win_thread.hpp:100:
// warning C4365: 'argument' : conversion from 'DWORD' to 'int',
//                             signed/unsigned mismatch

// boost/system/error_code.hpp:486:
// warning C4640: 's' : construction of local static object is not thread-safe

// vcxx8/VC/PlatformSDK/Include/BaseTsd.h:363:
// warning C4826: Conversion from 'const void *' to 'void *'
//                is sign-extended. This may cause unexpected runtime behavior.

#ifdef _MSC_VER
# pragma warning(disable:                       \
                 4191                           \
                 4242                           \
                 4365                           \
                 4628                           \
                 4640                           \
                 4826                           \
  )
#endif
