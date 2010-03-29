/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

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
    size_t ss1 = s1.size();
    size_t ss2 = s2.size();
    boost::multi_array<size_t, 2> d(boost::extents[ss1+1][ss2+1]);

    for (size_t i = 0; i <= ss1; ++i)
      d[i][0] = i;
    for (size_t j = 1; j <= ss2; ++j)
      d[0][j] = j;

    for (size_t i = 1; i <= ss1; ++i)
      for (size_t j = 1; j <= ss2; ++j)
      {
	size_t cost = s1[i-1] == s2[j-1] ? 0 : 1;
	d[i][j] = min(d[i-1][j] + 1,        // Deletion
                      d[i][j-1] + 1,        // Insertion
                      d[i-1][j-1] + cost);  // Substitution
	if (i > 1 && j > 1 && s1[i-1] == s2[j-2] && s1[i-2] == s2[j-1])
	  d[i][j] = min(d[i][j],
                        d[i-2][j-2] + cost); // Transposition
      }
    return d[ss1][ss2];
  }
}
