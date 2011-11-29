/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/bind.hh>
#include <libport/foreach.hh>
#include <libport/hash.hh>
#include <libport/symbol.hh>
#include "test.hh"

using boost::bind;
using libport::test_suite;

template <typename K, typename V>
std::ostream&
operator<<(std::ostream& o, const boost::unordered_map<K, V>& m)
{
  typedef typename boost::unordered_map<K, V>::value_type value_type;
  foreach (const value_type& p, m)
    o << p.first << " -> " << p.second << std::endl;
  return o;
}

template <typename T>
void
test_hash_map()
{
  boost::unordered_map<T, std::string> map;

  BOOST_CHECK(map.empty());
  BOOST_CHECK_EQUAL(map.size(), 0u);

  BOOST_CHECK_NO_THROW(map[T("foo")] = "foo");
  std::cerr << (map);
  BOOST_CHECK(!map.empty());
  BOOST_CHECK_EQUAL(map.size(), 1u);
  BOOST_CHECK_EQUAL(map[T("foo")], "foo");
  std::cerr << (map);

  BOOST_CHECK_NO_THROW(map[T("bar")] = "bar");
  std::cerr << (map);
  BOOST_CHECK(!map.empty());
  BOOST_CHECK_EQUAL(map.size(), 2u);
  BOOST_CHECK_EQUAL(map[T("foo")], "foo");
  BOOST_CHECK_EQUAL(map[T("bar")], "bar");

  BOOST_CHECK_NO_THROW(map[T("bar")] = "baz");
  BOOST_CHECK(!map.empty());
  BOOST_CHECK_EQUAL(map.size(), 2u);
  BOOST_CHECK_EQUAL(map[T("foo")], "foo");
  BOOST_CHECK_EQUAL(map[T("bar")], "baz");
  std::cerr << (map);

  BOOST_CHECK_NO_THROW(map.erase(T("foo")));
  std::cerr << (map);
  BOOST_CHECK(!map.empty());
  BOOST_CHECK_EQUAL(map.size(), 1u);
  BOOST_CHECK_EQUAL(map[T("bar")], "baz");

  BOOST_CHECK_NO_THROW(map.erase(T("bar")));
  std::cerr << (map);
  BOOST_CHECK(map.empty());
  BOOST_CHECK_EQUAL(map.size(), 0u);
}


test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("Libport.Hash");

# define def(Type)                                            \
  suite->add(BOOST_TEST_CASE(&test_hash_map<Type>))

  def(std::string);
  def(libport::Symbol);
  // Don't check on char*.
  //
  // There is no hash function for char*.  Since the test is using
  // const strings, GCC is certainly keeping a single copy of them
  // all, so since "foo" == "foo", of course, the hash table works,
  // but that's merely "chance".  It turns out that Visual does not
  // factor the strings (well, that's my understanding), so "of
  // course" the hash "does not work": it hashes on the addresses, not
  // the contents.  Since we don't use hashes on char*, let's not
  // force an interpretation at the library level (after all, we might
  // want to hash char* like any T*), just don't test char*.
# undef def
  return suite;
}
