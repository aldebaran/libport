/**
 ** Test code for libport/symbol.hh features.
 */

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <libport/unit-test.hh>

#include <libport/symbol.hh>

using libport::Symbol;
using libport::test_suite;

// Checking symbol.
void
check_symbols()
{
  const Symbol toto1("toto");
  const Symbol toto2("toto");
  const Symbol titi1("titi");

  BOOST_CHECK_EQUAL(Symbol::string_map_size (), 2);
  BOOST_CHECK_EQUAL(toto1.name_get (), "toto");

  BOOST_CHECK_EQUAL(toto1, Symbol("toto"));
  BOOST_CHECK(toto1 != Symbol("titi"));

  BOOST_CHECK_EQUAL(toto1, toto2);
  BOOST_CHECK(toto1 != titi1);

  std::string junk = "tata";
  const Symbol tata1(junk);
  junk = "toto";
  BOOST_CHECK_EQUAL(tata1, Symbol("tata"));
}

// Make sure we never create a Symbol that already exists.
void
check_fresh()
{
  // We rely on the knowledge that the fresh Symbols are
  // "<ROOT>_<COUNTER>", with COUNTER which is global and starts at 0.
  const Symbol a0 = Symbol::fresh(Symbol("a"));
  BOOST_CHECK_EQUAL(a0, Symbol("a_0"));
  const Symbol a1("a_1");
  const Symbol a2 = Symbol::fresh(Symbol("a"));
  BOOST_CHECK_EQUAL(a2, Symbol("a_2"));
}

void
check_serialization()
{
  std::stringstream s;
  Symbol foo("foo"), bar("bar");
  boost::archive::text_oarchive oa(s);
  oa << foo << foo << bar;

  boost::archive::text_iarchive ia(s);
  Symbol s1, s2, s3;
  ia >> s1 >> s2 >> s3;

  BOOST_CHECK_EQUAL(s1, s2);
  BOOST_CHECK(s1 != s3);
  BOOST_CHECK_EQUAL(s1.name_get(), "foo");
  BOOST_CHECK_EQUAL(s3.name_get(), "bar");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::Symbol test suite");
  suite->add(BOOST_TEST_CASE(check_symbols));
  suite->add(BOOST_TEST_CASE(check_fresh));
  suite->add(BOOST_TEST_CASE(check_serialization));
  return suite;
}
