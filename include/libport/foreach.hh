/**
 ** \file libport/foreach.hh
 ** \brief Syntactic sugar for \a BOOST_FOREACH.
 */

#ifndef LIBPORT_FOREACH_HH
# define LIBPORT_FOREACH_HH

# include <boost/foreach.hpp>

/// \def foreach
/// \brief A shortcut for \a BOOST_FOREACH.
///
/// Not to be confused with libport::for_each.
# define foreach BOOST_FOREACH

#endif // !LIBPORT_FOREACH_HH
