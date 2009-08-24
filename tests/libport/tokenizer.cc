#include <boost/bind.hpp>
#include <libport/foreach.hh>
#include <libport/lexical-cast.hh>
#include <libport/tokenizer.hh>
#include <libport/unit-test.hh>

using boost::bind;
using libport::test_suite;

namespace std
{
  std::ostream&
  operator<<(std::ostream& o, const libport::tokenizer_type& tok)
  {
    std::string sep;
    foreach (const std::string& line, tok)
    {
      o << sep << line;
      sep = ",";
    }
    return o;
  }
}

void
check (const std::string& in,
       const std::string& out_strip,
       const std::string& out_kept)
{
  libport::tokenizer_type tok = libport::make_tokenizer(in, "\n");
  BOOST_CHECK_EQUAL(string_cast(tok), out_strip);

  tok = libport::make_tokenizer(in, "\n",
                                "", boost::keep_empty_tokens);
  BOOST_CHECK_EQUAL(string_cast(tok), out_kept);

  tok = libport::lines(in);
  BOOST_CHECK_EQUAL(string_cast(tok), out_kept);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::tokenizer");

#define CASE(In, Stripped, Full)                                \
  suite->add(BOOST_TEST_CASE(bind(check, In, Stripped, Full)));

  CASE("abc",            "abc",     "abc");
  CASE("abc\n\n123",     "abc,123", "abc,,123");
  CASE("abc\n123",       "abc,123", "abc,123");
  CASE("abc\n123\n",     "abc,123", "abc,123,");
  CASE("\nabc\n\n123\n", "abc,123", ",abc,,123,");
  return suite;
}
