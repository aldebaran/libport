#ifndef LIBORT_RESERVED_VECTOR_HH
# define LIBPORT_RESERVED_VECTOR_HH

# include <vector>

namespace libport
{
  /** Vector class that reserves \b R elements in constructor to improve
    * performances.
    */
  template<class T, int R=4> class ReservedVector
  : public std::vector<T>
  {
  public:
    template<typename I>
    ReservedVector(I b, I e)
    :std::vector<T>(b, e) {}
    ReservedVector() { this->reserve(R);}
    template<int R2>
    ReservedVector(const ReservedVector<T, R2>& b)
    : std::vector<T>(b) {}
  };

  /// Unefficient pop_front for vector-like types.
  template<typename T>
  void pop_front(T& v)
  {
    T v2(++v.begin(), v.end());
    v = v2;
  }

  /// Unefficient push_front for vector-like types.
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
