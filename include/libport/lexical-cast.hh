/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** \file libport/lexical-cast.hh
 ** \brief Syntactic sugar for \a boost::lexical_cast.
 */

#ifndef LIBPORT_LEXICAL_CAST_HH
# define LIBPORT_LEXICAL_CAST_HH

# include <string>

# include <libport/system-warning-push.hh>
#  include <boost/lexical_cast.hpp>
# include <libport/system-warning-pop.hh>

// dynamic_cast and so forth are not in some scope.  So put these casts
// here too.
using boost::lexical_cast;

// Implemented as a macro to avoid having to provide different
// implementations, depending on the argument passing method.
# define string_cast lexical_cast<std::string>

#endif // !LIBPORT_LEXICAL_CAST_HH
