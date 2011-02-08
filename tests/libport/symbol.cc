/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** Test code for libport/symbol.hh features.
 */

#include <sstream>

#include <libport/config.h>

#ifdef WITH_BOOST_SERIALIZATION
# include <boost/archive/text_oarchive.hpp>
# include <boost/archive/text_iarchive.hpp>
#endif // WITH_BOOST_SERIALIZATION
#include <libport/unit-test.hh>

#include <libport/symbol.hh>

using libport::Symbol;
using libport::test_suite;


// Checking symbol.
void
check_symbols()
{
  // This value is used to get the size of Symbol map because the libport
  // library may use some for itself.  We just hope that the dummy names
  // used in this test do not collide with the name used in the library.
  const unsigned int init_map_size = Symbol::string_map_size ();

  const Symbol foo1("foo");
  const Symbol foo2("foo");
  const Symbol bar1("bar");

  BOOST_CHECK_EQUAL(Symbol::string_map_size () - init_map_size, 2u);
  BOOST_CHECK_EQUAL(foo1.name_get (), "foo");

  BOOST_CHECK_EQUAL(foo1, Symbol("foo"));
  BOOST_CHECK(foo1 != Symbol("bar"));

  BOOST_CHECK_EQUAL(foo1, foo2);
  BOOST_CHECK(foo1 != bar1);

  std::string junk = "baz";
  const Symbol baz1(junk);
  junk = "foo";
  BOOST_CHECK_EQUAL(baz1, Symbol("baz"));
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
  std::string a2 = Symbol::fresh_string("a");
  BOOST_CHECK_EQUAL(a2, "a_2");
}

void
check_serialization()
{
#ifdef WITH_BOOST_SERIALIZATION
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
#endif // WITH_BOOST_SERIALIZATION
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
