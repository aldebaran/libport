/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBORT_RESERVED_VECTOR_HH
# define LIBPORT_RESERVED_VECTOR_HH

# include <vector>

namespace libport
{
  /** Vector class that reserves \b R elements in constructor to improve
    * performances.
    */
  template<class T, int R=4>
  class ReservedVector
    : public std::vector<T>
  {
  public:
    ReservedVector();

    template<typename I1, typename I2>
    ReservedVector(I1 b, I2 e);

    template<int R2>
    ReservedVector(const ReservedVector<T, R2>& b);
  };

  /// Unefficient pop_front for vector-like types.
  template<typename T>
  void pop_front(T& v);

  /// Unefficient push_front for vector-like types.
  template<typename T, typename U>
  void push_front(T& v, const U& e);
}

# include <libport/reserved-vector.hxx>

#endif
