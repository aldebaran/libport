/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/// \file libport/cast.hh
/// \brief Libport.Cast.

#ifndef LIBPORT_CAST_HH
# define LIBPORT_CAST_HH

# include <libport/lexical-cast.hh>

/// Convert \a in from \a In to \a Out via a union, so that the
/// compiler does not feel it is cheated.
///
/// Left in :: on purpose.
template <typename In, typename Out>
Out union_cast(In in);

# include <libport/cast.hxx>

#endif // !LIBPORT_CAST_HH
