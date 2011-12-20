/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/bind.hh>
#include <libport/foreach.hh>
#include <libport/lexical-cast.hh>
#include <libport/tokenizer.hh>
#include <libport/unit-test.hh>

using boost::bind;
using libport::test_suite;

namespace std
{
  // In order to be able to use string_cast.
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
#define CHECK(What, Out)                        \
  BOOST_CHECK_EQUAL(string_cast(What), Out)

  CHECK(libport::make_tokenizer(in, "\n"),
        out_strip);
  CHECK(libport::make_tokenizer(in, "\n", "", boost::keep_empty_tokens),
        out_kept);
  CHECK(libport::lines(in), out_kept);
#undef CHECK
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

  // Try with a much larger string.
  {
    std::stringstream in;
    std::stringstream out;
    for (int i = 0; i < 1000; ++i)
    {
      in  << i << std::endl;
      out << (i ? "," : "") << i;
    }
    CASE(in.str(), out.str(), out.str() + ",");
  }
  return suite;
}
