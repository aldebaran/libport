/**
 ** \file libport/foreach.hh
 ** \brief Syntactic sugar for \a BOOST_FOREACH.
 */

#ifndef LIBPORT_FOREACH_HH
# define LIBPORT_FOREACH_HH

# include <libport/range.hh>

# include <boost/foreach.hpp>

/// \def foreach
/// \brief A shortcut for \a BOOST_FOREACH.
///
/// Not to be confused with libport::for_each.
// No redefinition if already defined by other libraries.
#ifndef foreach
# define foreach BOOST_FOREACH
#endif

/// \def rforeach(VarDec, Sequence)
/// \brief Same as foreach, but in reversed order.
# define rforeach(Variable, Container)                                  \
  BOOST_FOREACH(Variable, libport::make_reverse_iterator_range(Container))

#endif // !LIBPORT_FOREACH_HH
