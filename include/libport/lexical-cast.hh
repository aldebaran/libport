/**
 ** \file libport/lexical-cast.hh
 ** \brief Syntactic sugar for \a boost::lexical_cast.
 */

#ifndef LIBPORT_LEXICAL_CAST_HH
# define LIBPORT_LEXICAL_CAST_HH

# include <string>
# include <boost/lexical_cast.hpp>

// dynamic_cast and so forth are not in some scope.  So put these casts
// here too.
using boost::lexical_cast;

// Implemented as a macro to avoid having to provide differents
// implementation, depending on the argument passing method.
# define string_cast lexical_cast<std::string>

#endif // !LIBPORT_LEXICAL_CAST_HH
