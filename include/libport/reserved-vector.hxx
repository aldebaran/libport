namespace libport
{
  template<class T, int R>
  ReservedVector<T, R>::ReservedVector()
  {
    this->reserve(R);
  }

  template<class T, int R>
  template<typename I1, typename I2>
  ReservedVector<T, R>::ReservedVector(I1 b, I2 e)
    : std::vector<T>(b, e)
  {}

  template<class T, int R>
  template<int R2>
  ReservedVector<T, R>::ReservedVector(const ReservedVector<T, R2>& b)
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
