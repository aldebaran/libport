/**
 ** Test command line interface tools.
 */

#include <libport/cli.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

void
check ()
{
  const std::string input =
    std::string() + getenv("SRCDIR") + "/libport/666.txt";
# define CHECK(Type, Value)                             \
  do {                                                  \
    Type v = libport::file_contents_get<Type>(input);   \
    BOOST_CHECK_EQUAL(v, Value);                        \
  } while (0)

  CHECK(int, 666);
  CHECK(unsigned, 666u);
  CHECK(std::string, "666");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::cli");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
