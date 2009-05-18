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
