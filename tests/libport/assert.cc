/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <sstream>

#include <libport/cassert>
#include <libport/unit-test.hh>

using libport::test_suite;
using namespace libport;

static
void
unreachable_1()
{
  return;
  unreachable();
}

template <typename T>
static
void
unreachable_2(T)
{
  return;
  // FIXME: GCC 4.4 makes a warning here  unreachable();
}

static
const char*
unreachable_3()
{
  enum count_type { one, two };
  count_type c = one;
  switch (c)
  {
  case one:
    return "1";
  case two:
    return "2";
  }
  unreachable();
}

void
check_assert_op()
{
  unreachable_1();
  unreachable_2(2);
  unreachable_3();

  // I'd like to check the error messages, but how?
  BOOST_CHECK_NO_THROW(assert_eq(0, 0));
  BOOST_CHECK_NO_THROW(assert_ge(2, -2));
  BOOST_CHECK_NO_THROW(assert_gt(2, -2));
  BOOST_CHECK_NO_THROW(assert_le(0.1, 0.1));
  BOOST_CHECK_NO_THROW(assert_lt(0.1, 0.2));
  BOOST_CHECK_NO_THROW(assert_ne("foo", "bar"));
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("cassert");
  suite->add(BOOST_TEST_CASE(check_assert_op));
  return suite;
}
