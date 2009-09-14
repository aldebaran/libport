/*
 * Copyright (C) 2009, Gostai S.A.S.
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

# include <libport/warning-push.hh>

// boost/asio/detail/win_thread.hpp:100:
// warning C4365: 'argument' : conversion from 'DWORD' to 'int',
//                             signed/unsigned mismatch

# pragma warning(disable:                       \
                 4365                           \
  )

#endif
