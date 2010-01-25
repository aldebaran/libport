/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/bind.hh>
#include <libport/hash.hh>
#include <libport/symbol.hh>
#include <libport/test.hh>

using boost::bind;
using libport::test_suite;

template <typename T>
void
test_hash_map()
{
  boost::unordered_map<T, std::string> map;

  BOOST_CHECK(map.empty());
  BOOST_CHECK_EQUAL(map.size(), 0u);

  BOOST_CHECK_NO_THROW(map[T("foo")] = "foo");
  BOOST_CHECK(!map.empty());
  BOOST_CHECK_EQUAL(map.size(), 1u);
  BOOST_CHECK_EQUAL(map[T("foo")], "foo");

  BOOST_CHECK_NO_THROW(map[T("bar")] = "bar");
  BOOST_CHECK(!map.empty());
  BOOST_CHECK_EQUAL(map.size(), 2u);
  BOOST_CHECK_EQUAL(map[T("foo")], "foo");
  BOOST_CHECK_EQUAL(map[T("bar")], "bar");

  BOOST_CHECK_NO_THROW(map[T("bar")] = "baz");
  BOOST_CHECK(!map.empty());
  BOOST_CHECK_EQUAL(map.size(), 2u);
  BOOST_CHECK_EQUAL(map[T("foo")], "foo");
  BOOST_CHECK_EQUAL(map[T("bar")], "baz");

  BOOST_CHECK_NO_THROW(map.erase(T("foo")));
  BOOST_CHECK(!map.empty());
  BOOST_CHECK_EQUAL(map.size(), 1u);
  BOOST_CHECK_EQUAL(map[T("bar")], "baz");

  BOOST_CHECK_NO_THROW(map.erase(T("bar")));
  BOOST_CHECK(map.empty());
  BOOST_CHECK_EQUAL(map.size(), 0u);
}


test_suite*
init_test_suite()
{
  // This test does not pass under Wine, for unknown reasons.  It did
  // not pass when we were on top of stdext::hash_map, it does not
  // pass either when we use Boost.Unordered.
  skip_if("Wine");

  test_suite* suite = BOOST_TEST_SUITE("libport::hash test suite");

# define def(Type)                                            \
  suite->add(BOOST_TEST_CASE(test_hash_map<Type>))

  def(const char*);
  def(char*);
  def(std::string);
  def(libport::Symbol);
# undef def
  return suite;
}
