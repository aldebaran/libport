/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_HASH_HH
# define LIBPORT_HASH_HH

# include <boost/version.hpp>
# include <libport/config.h>

# if BOOST_VERSION <= 103800 && LIBPORT_HAVE_PRAGMA_GCC_DIAGNOSTIC
#  pragma GCC diagnostic warning "-Wunused-parameter"
# endif

# include <boost/unordered_map.hpp>

#endif // !LIBPORT_HASH_HH
