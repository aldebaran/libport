/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#include <libport/cstdlib>
#include <libport/unit-test.hh>

using libport::test_suite;

static void test_environ()
{
  // Variables we know to be defined.
  const char* path = getenv("PATH");
  BOOST_CHECK(!!path);
  BOOST_CHECK(!!getenv("SRCDIR"));

  // setenv should not override by default.
  BOOST_CHECK(!setenv("PATH", "TOTO", 0));
  BOOST_CHECK_EQUAL(getenv("PATH"), path);

  // Variables we know (hope) not to be defined.
  const char* var = "__THIS_IS_AN_UNLIKELY_VARIABLE_NAME__";
  BOOST_CHECK(!getenv(var));
  BOOST_CHECK(!setenv(var, "23", 0));
  BOOST_CHECK_EQUAL("23", getenv(var));
  BOOST_CHECK(!unsetenv(var));
  BOOST_CHECK(!getenv(var));
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::cstdlib");
  suite->add(BOOST_TEST_CASE(test_environ));
  return suite;
}
