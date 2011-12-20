/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>
#include <libport/bind.hh>
#include <libport/unit-test.hh>

using boost::bind;
using libport::test_suite;

#include <libport/statistics.hh>

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
  BOOST_TEST_MESSAGE("Mean: " << s.mean());
  BOOST_TEST_MESSAGE("Variance: " << s.variance());
  BOOST_CHECK_EQUAL(size, s.size());
  BOOST_CHECK_EQUAL(min, s.min());
  BOOST_CHECK_EQUAL(max, s.max());
  BOOST_CHECK_LT(_fabs(mean - s.mean()), 1e-4);
  BOOST_CHECK_LT(_fabs(variance - s.variance()), 1e-4);
}

template<typename R>
static void
check1(size_t capacity, unsigned int size,
       unsigned int min, unsigned int max,
       R mean, R variance)
{
  typedef libport::Statistics<unsigned int, R> stat_type;
  BOOST_CHECK_NO_THROW(stat_type(capacity));
  stat_type s(capacity);
  BOOST_CHECK_NO_THROW(s.add_sample(2));
  BOOST_CHECK_NO_THROW(s.add_sample(2));
  BOOST_CHECK_NO_THROW(s.add_sample(3));
  BOOST_CHECK_NO_THROW(s.add_sample(4));
  BOOST_CHECK_NO_THROW(s.add_sample(7));
  check(s, size, min, max, mean, variance);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::statistics");

#define TEST(Args)                                      \
  suite->add(BOOST_TEST_CASE(boost::bind Args));

  // Running samples
  TEST((check1<unsigned int>, 3, 3, 3, 7, 4, 3));
  TEST((check1<double>, 3, 3, 3, 7, 4.666666666, 2.8888888888));

  // Non-running samples
  TEST((check1<unsigned int>, 0, 5, 2, 7, 3, 3));
  TEST((check1<double>, 0, 5, 2, 7, 3.6, 3.44));

  // Running samples with non-full buffer
  TEST((check1<unsigned int>, 10, 5, 2, 7, 3, 3));
  TEST((check1<double>, 10, 5, 2, 7, 3.6, 3.44));
  return suite;
}
