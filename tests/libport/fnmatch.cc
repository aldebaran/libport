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

#ifdef _MSC_VER
# pragma comment(lib, "shlwapi.lib")
#endif

using libport::test_suite;

static void test_fnmatch()
{
  BOOST_CHECK_EQUAL(libport::fnmatch("b*", "foo"), FNM_NOMATCH);
  BOOST_CHECK_EQUAL(libport::fnmatch("b*", "bar"), 0);
  BOOST_CHECK_EQUAL(libport::fnmatch("b*", "baz"), 0);
  BOOST_CHECK_EQUAL(libport::fnmatch("*f", "foo"), FNM_NOMATCH);
  BOOST_CHECK_EQUAL(libport::fnmatch("*f*", "foo"), 0);
  BOOST_CHECK_EQUAL(libport::fnmatch("*b*", "foobar"), 0);
  BOOST_CHECK_EQUAL(libport::fnmatch("*foo*", "barfoo"), 0);
  BOOST_CHECK_EQUAL(libport::fnmatch("*foo*", "barfoobaz"), 0);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::fnmatch");
  suite->add(BOOST_TEST_CASE(test_fnmatch));
  return suite;
}
