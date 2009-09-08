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

// Testing system under windows is a pain. We're probably not actually
// going to use it anyway ...
#ifndef WIN32
static void test_system()
{
  // Unfortunately, boost::unit_test implodes if any child has a non null
  // return value, so we cannot test the failing case.

  BOOST_CHECK_EQUAL(libport::system("true"), 0);
  // BOOST_CHECK_EQUAL(libport::system("false"), 1);
  BOOST_CHECK_NO_THROW(int ignored = libport::system("true"); (void)ignored);
  // BOOST_CHECK_THROW(int ignored = libport::system("false"); (void)ignored,
  //                   std::exception);
}
#endif

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::cstdlib");
  suite->add(BOOST_TEST_CASE(test_environ));

#ifndef WIN32
  suite->add(BOOST_TEST_CASE(test_system));
#endif
  return suite;
}
