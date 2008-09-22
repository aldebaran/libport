#include <cassert>
#include <libport/statistics.hh>

#include <iostream>

template<typename T, bool extra_precision>
static void
check(const libport::Statistics<T, extra_precision>& s,
      size_t size, T min, T max, unsigned int mean, double variance)
{
  std::cerr << "Mean: " << s.mean() << std::endl;
  std::cerr << "Variance: " << s.variance() << std::endl;
  assert(size == s.size());
  assert(min == s.min());
  assert(max == s.max());
  assert(mean == s.mean());
  assert(fabs(variance - s.variance()) < 1e-4);
}

template<bool extra_precision>
static void
check1(size_t capacity, unsigned int size, unsigned int min, unsigned int max, unsigned int mean, double variance)
{
  libport::Statistics<unsigned int, extra_precision> s(capacity);
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
  check1<false>(3, 3, 3, 7, 4, 2.888888888888);
  check1<true>(3, 3, 3, 7, 5, 2.8888888888);

  // Non-running samples
  check1<false>(0, 5, 2, 7, 3, 3.44);
  check1<true>(0, 5, 2, 7, 4, 3.44);

  // Running samples with non-full buffer
  check1<false>(10, 5, 2, 7, 3, 3.44);
  check1<true>(10, 5, 2, 7, 4, 3.44);
}
