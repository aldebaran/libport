#include <libport/reserved-vector.hh>

namespace libport
{
  template<typename I>
  template<class T, int R>
  ReservedVector::ReservedVector(I b, I e)
    :std::vector<T>(b, e)
  {}

  template<class T, int R>
  ReservedVector::ReservedVector()
  {
    this->reserve(R);
  }

  template<int R2>
  template<class T, int R>
  ReservedVector::ReservedVector(const ReservedVector<T, R2>& b)
    : std::vector<T>(b)
  {}

  template<typename T>
  void pop_front(T& v)
  {
    T v2(++v.begin(), v.end());
    v = v2;
  }

  template<typename T, typename U>
  void push_front(T& v, const U& e)
  {
    T v2;
    v2.push_back(e);
    v2.insert(v2.end(), v.begin(), v.end());
    v = v2;
  }
}

#endif
