/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

namespace libport
{
  template<class T, int R>
  inline
  ReservedVector<T, R>::ReservedVector()
  {
    this->reserve(R);
  }

  template<class T, int R>
  template<typename I1, typename I2>
  inline
  ReservedVector<T, R>::ReservedVector(I1 b, I2 e)
    : std::vector<T>(b, e)
  {
    this->reserve(R);
  }

  template<class T, int R>
  template<int R2>
  inline
  ReservedVector<T, R>::ReservedVector(const ReservedVector<T, R2>& b)
    : std::vector<T>(b)
  {
    this->reserve(R);
  }

  template<typename T>
  inline
  void pop_front(T& v)
  {
    T v2(++v.begin(), v.end());
    v = v2;
  }

  template<typename T, typename U>
  inline
  void push_front(T& v, const U& e)
  {
    T v2;
    v2.push_back(e);
    v2.insert(v2.end(), v.begin(), v.end());
    v = v2;
  }
}
