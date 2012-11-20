/*
 * Copyright (C) 2008-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/foreach.hh
/// \brief Syntactic sugar for \a BOOST_FOREACH.

#ifndef LIBPORT_FOREACH_HH
# define LIBPORT_FOREACH_HH

/// \def foreach
/// \brief A shortcut for \a BOOST_FOREACH.
///
/// Not to be confused with libport::for_each.

// No redefinition if already defined by other libraries.
//
// It would seem saner to declare this macro *after* having included
// the definition of BOOST_FOREACH, but BOOST_FOREACH is actually
// buggy, and uses the identifier "foreach" as a namespace on
// occasion.  And unfortunately, this shows (see
// http://thread.gmane.org/gmane.comp.lib.boost.user/71410, and
// https://svn.boost.org/trac/boost/ticket/6131).
//
// Luckily, Boost.Foreach is header-only, so one way out is to make
// sure we use BOOST_FOREACH consistently *everywhere*, so define
// foreach before including the header.
//
// And never use boost/foreach.hpp, only libport/foreach.hh.

# include <boost/version.hpp>
# if 104800 < BOOST_VERSION
#  define LIBPORT_FOREACH_INCLUDE_FIRST 1
# else
#  define LIBPORT_FOREACH_INCLUDE_FIRST 0
# endif

# if LIBPORT_FOREACH_INCLUDE_FIRST
#  include <libport/system-warning-push.hh>
#   include <boost/foreach.hpp>
#  include <libport/system-warning-pop.hh>
# endif

# ifndef foreach
#  define foreach BOOST_FOREACH
# endif

/// \def rforeach
/// \brief Shortcut for \a BOOST_REVERSE_FOREACH
# define rforeach BOOST_REVERSE_FOREACH

# if ! LIBPORT_FOREACH_INCLUDE_FIRST
#  include <libport/system-warning-push.hh>
#   include <boost/foreach.hpp>
#  include <libport/system-warning-pop.hh>
# endif

#endif // !LIBPORT_FOREACH_HH
