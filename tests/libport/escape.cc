/**
 ** Test code for libport/escape.hh.
 */

#include <sstream>
#include <libport/escape.hh>
#include <libport/unit-test.hh>

using libport::escape;
using libport::test_suite;

void
check ()
{
# define CHECK(In, Out)                         \
  do {                                          \
    std::ostringstream o;                       \
    o << In;                                    \
    BOOST_CHECK_EQUAL(o.str(), Out);            \
  } while (0)

  CHECK(escape("\a\b\f\n\r\t\v\\\""), "\\x07\\b\\f\\n\\r\\t\\v\\\\\\\"");

  CHECK(escape("\""), "\\\"");
  CHECK(escape("'"), "'");

  CHECK(escape("'",  '\''), "\\\'");
  CHECK(escape("\"", '\''), "\"");
}


test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::escape");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
