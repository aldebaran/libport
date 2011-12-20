/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/format.hh>
#include <libport/unit-test.hh>

using libport::test_suite;
using namespace libport;

void
check_numbers()
{
#define CHECK(Out, ...)                                 \
  BOOST_CHECK_EQUAL(libport::format(__VA_ARGS__), Out)
  // Check that small integer values are always displayed as integers
  // with %s.
  CHECK("1",   "%s", 1);
  CHECK("1",   "%s", 1);
  CHECK("1",   "%s", 1.0);
  CHECK("1",   "%s", 1.0);

  CHECK("4294967296",   "%s", 4294967296ll);
  CHECK("21474836470",  "%s", 21474836470ll);
  // std::numeric_limits<long long>::max()
  CHECK("9223372036854775807",  "%s", 9223372036854775807ll);
  // std::numeric_limits<long long>::min(), but GCC does not accept it
  // as input.
  CHECK("-9223372036854775808",  "%s", std::numeric_limits<long long>::min());

  // Too big, the result is in scientific notation.
  // CHECK("4294967296",   "%s", 4294967296.0);
  // CHECK("21474836470",  "%s", 21474836470.0);

  CHECK("1",   "%d", 1.0);
  CHECK("1.000000", "%f", 1.0);
  CHECK("1",   "%d", 1);
  CHECK("1",   "%f", 1);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE(__FILE__);
  suite->add(BOOST_TEST_CASE(check_numbers));
  return suite;
}
