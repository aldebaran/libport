/*
 * Copyright (C) 2010, Gostai S.A.S.
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

  CHECK("4294967296",   "%s", 4294967296);

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
