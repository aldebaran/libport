/**
 ** \file libport/foreach.hh
 ** \brief Find Boost.Foreach
 */

#ifndef LIBPORT_FOREACH_HH
# define LIBPORT_FOREACH_HH

# include <boost/version.hpp>

# ifndef BOOST_VERSION
#  error Could not find your version of Boost.
# endif

# if BOOST_VERSION == 103301
// In 1.33.1 Boost.Foreach was included but as a test
// The version is slightly less optimized than that of >= 1.34 but it will do.
#  include <boost/test/utils/foreach.hpp>
# elif BOOST_VERSION > 103301
#  include <boost/foreach.hpp>
# else
#  error Your version of Boost is too old to have Boost.Foreach
# endif

#endif // !LIBPORT_FOREACH_HH
