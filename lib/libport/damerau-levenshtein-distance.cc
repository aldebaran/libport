#include <boost/multi_array.hpp>
#include <libport/export.hh>
#include <libport/damerau-levenshtein-distance.hh>

namespace libport
{
  using std::min;

  template <typename T>
  static
  T min(T t1, T t2, T t3)
  {
    return min(t1, min(t2, t3));
  }

  size_t
  damerau_levenshtein_distance(const std::string& s1, const std::string& s2)
  {
    boost::multi_array<size_t, 2> d(boost::extents[s1.size()+1][s2.size()+1]);

    for (size_t i = 0; i <= s1.size(); ++i)
      d[i][0] = i;
    for (size_t j = 1; j <= s2.size(); ++j)
      d[0][j] = j;

    for (size_t i = 1; i <= s1.size(); ++i)
      for (size_t j = 1; j <= s2.size(); ++j)
      {
	size_t cost = s1[i-1] == s2[j-1] ? 0 : 1;
	d[i][j] = min(d[i-1][j] + 1,        // Deletion
                      d[i][j-1] + 1,        // Insertion
                      d[i-1][j-1] + cost);  // Substitution
	if (i > 1 && j > 1 && s1[i-1] == s2[j-2] && s1[i-2] == s2[j-1])
	  d[i][j] = min(d[i][j],
                        d[i-2][j-2] + cost); // Transposition
      }
    return d[s1.size()][s2.size()];
  }
}

