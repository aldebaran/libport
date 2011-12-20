/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cmath>
#include <libport/unit-test.hh>

using libport::test_suite;
using namespace libport;

template <typename T>
void
check_round()
{
#define CHECK(In, Out)                                  \
  BOOST_CHECK_EQUAL(libport::round(T(In)), T(Out))
  CHECK( 0.0,  0.);
  CHECK( 1.4,  1.);
  CHECK( 1.5,  2.);
  CHECK( 1.6,  2.);
  CHECK(-1.4, -1.);
  CHECK(-1.5, -2.);
  CHECK(-1.6, -2.);

  T inf = std::numeric_limits<T>::infinity();
  CHECK( inf,  inf);
  CHECK(-inf, -inf);
#undef CHECK
  T nan = std::numeric_limits<T>::quiet_NaN();
  BOOST_CHECK(std::isnan(libport::round(nan)));
}

// Needed by VCXX 2005.  Forcing the instantiation does not even suffice.
typedef long double long_double;
#define DEFINE(Type)                            \
  void                                          \
  check_ ## Type()                              \
  {                                             \
    check_round<Type>();                        \
  }
DEFINE(float);
DEFINE(double);
DEFINE(long_double);
#undef DEFINE

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE(__FILE__);
  suite->add(BOOST_TEST_CASE(check_float));
  suite->add(BOOST_TEST_CASE(check_double));
  suite->add(BOOST_TEST_CASE(check_long_double));
  return suite;
}
