/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/foreach.hh
 ** \brief Syntactic sugar for \a BOOST_FOREACH.
 */

#ifndef LIBPORT_FOREACH_HH
# define LIBPORT_FOREACH_HH

# include <libport/system-warning-push.hh>
#  include <boost/foreach.hpp>
# include <libport/system-warning-pop.hh>

/// \def foreach
/// \brief A shortcut for \a BOOST_FOREACH.
///
/// Not to be confused with libport::for_each.
// No redefinition if already defined by other libraries.
#ifndef foreach
# define foreach BOOST_FOREACH
#endif

/// \def rforeach
/// \brief Shortcut for \a BOOST_REVERSE_FOREACH
# define rforeach BOOST_REVERSE_FOREACH

#endif // !LIBPORT_FOREACH_HH
