#include <libport/cstdlib>
#include <libport/unit-test.hh>

using libport::test_suite;

static void environ()
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
  suite->add(BOOST_TEST_CASE(environ));
  return suite;
}
