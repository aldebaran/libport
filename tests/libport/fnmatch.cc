/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/fnmatch.h>
#include <libport/unit-test.hh>
#include <libport/instrument.hh>

using libport::test_suite;

static void test_fnmatch()
{
  BOOST_CHECK(!libport::fnmatch("b*", "foo"));
  BOOST_CHECK(libport::fnmatch("b*", "bar"));
  BOOST_CHECK(libport::fnmatch("b*", "baz"));
  BOOST_CHECK(!libport::fnmatch("*f", "foo"));
  BOOST_CHECK(libport::fnmatch("*f*", "foo"));
  BOOST_CHECK(libport::fnmatch("*b*", "foobar"));
  BOOST_CHECK(libport::fnmatch("*foo*", "barfoo"));
  BOOST_CHECK(libport::fnmatch("*foo*", "barfoobaz"));
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::fnmatch");
  suite->add(BOOST_TEST_CASE(test_fnmatch));
  return suite;
}
