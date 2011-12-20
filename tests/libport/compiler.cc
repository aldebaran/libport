/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/compiler.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

static
int
ATTRIBUTE_USED
forty_two (int foo, int bar)
{
  LIBPORT_USE(foo);
  LIBPORT_USE(foo, bar);
  return 21;
}

// Boost.Test does not like empty test suites.
static void test()
{
  BOOST_CHECK(true);
}


test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("Libport.Compiler test suite");
  suite->add(BOOST_TEST_CASE(test));
  return suite;
}
