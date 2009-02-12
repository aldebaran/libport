#include <boost/bind.hpp>
#include <libport/foreach.hh>
#include <libport/tokenizer.hh>
#include <libport/unit-test.hh>

using boost::bind;
using libport::test_suite;

void
check (const std::string& in, const std::string& out)
{
  std::string res;

  boost::tokenizer< boost::char_separator<char> > tok =
    libport::make_tokenizer(in, "\n");
  std::string sep;
  foreach (const std::string& line, tok)
  {
    res += sep + line;
    sep = ",";
  }

  BOOST_CHECK_EQUAL(res, out);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::tokenizer");

#define CASE(In, Out)                                   \
  suite->add(BOOST_TEST_CASE(bind(check, In, Out)));

  CASE("abc", "abc");
  CASE("abc\n\n123", "abc,123");
  CASE("abc\n123", "abc,123");
  CASE("abc\n123\n", "abc,123");
  CASE("\nabc\n\n123\n", "abc,123");
  return suite;
}
