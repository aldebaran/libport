/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_HASH_HH
# define LIBPORT_HASH_HH

// The following is useless.
//
// It was an attempt to fix a warning in Boost.Unordered that is not
// fixed yet, but appeared when we moved to the older 1.38.  Actually
// the problem was that we picked up the Boost in /opt/local/include
// because X11_CPPFLAGS (needed for programs such as
// urbi-balltracking) included "-I /opt/local/include".  And since we
// had "-isystem .../boost-1.38" and system dirs were treated last,
// (1) we were including the wrong copy of boost, and (2) it was
// included via -I instead of -isystem, whcih resulted in many useless
// warnings.
//
// The following pragma was adressing one such warning.
//
// But since then we also use -isystem for X11_CPPFLAGS, so (1) we use
// the right Boost, (2) whose warnings are covered by -isystem.

#if 0
#  include <boost/version.hpp>
#  include <libport/config.h>

#  if BOOST_VERSION <= 104100 && LIBPORT_HAVE_PRAGMA_GCC_DIAGNOSTIC
#   pragma GCC diagnostic warning "-Wunused-parameter"
#  endif
# endif

// Boost header below is including cmath so include our wrapper first.
# include <libport/cmath>
# include <boost/unordered_map.hpp>

#endif // !LIBPORT_HASH_HH
