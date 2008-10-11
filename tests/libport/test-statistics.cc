#include <cassert>
#include <libport/statistics.hh>

#include <iostream>

// Visual C++ gets confused if we use "fabs" in the template below, as
// it is an overloaded operator.
static double
_fabs(double d)
{
  return fabs(d);
}

template<typename T, typename R>
static void
check(const libport::Statistics<T, R>& s,
      size_t size, T min, T max, R mean, R variance)
{
  std::cerr << "Mean: " << s.mean() << std::endl;
  std::cerr << "Variance: " << s.variance() << std::endl;
  assert(size == s.size());
  assert(min == s.min());
  assert(max == s.max());
  assert(_fabs(mean - s.mean()) < 1e-4);
  assert(_fabs(variance - s.variance()) < 1e-4);
}

template<typename R>
static void
check1(size_t capacity, unsigned int size, unsigned int min, unsigned int max, R mean, R variance)
{
  libport::Statistics<unsigned int, R> s(capacity);
  s.add_sample(2);
  s.add_sample(2);
  s.add_sample(3);
  s.add_sample(4);
  s.add_sample(7);
  check(s, size, min, max, mean, variance);
}

int
main()
{
  // Running samples
  check1<unsigned int>(3, 3, 3, 7, 4, 3);
  check1<double>(3, 3, 3, 7, 4.666666666, 2.8888888888);

  // Non-running samples
  check1<unsigned int>(0, 5, 2, 7, 3, 3);
  check1<double>(0, 5, 2, 7, 3.6, 3.44);

  // Running samples with non-full buffer
  check1<unsigned int>(10, 5, 2, 7, 3, 3);
  check1<double>(10, 5, 2, 7, 3.6, 3.44);
}
