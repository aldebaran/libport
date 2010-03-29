/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_DAMERAU_LEVENSHTEIN_DISTANCE_HH
# define LIBPORT_DAMERAU_LEVENSHTEIN_DISTANCE_HH

# include <string>
# include <libport/export.hh>

namespace libport
{
  /// Return the distance between \a s1 and \a s2.
  LIBPORT_API
  size_t
  damerau_levenshtein_distance(const std::string& s1, const std::string& s2);
}

#endif // !LIBPORT_DAMERAU_LEVENSHTEIN_DISTANCE_HH
