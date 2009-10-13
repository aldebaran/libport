/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/test.hh>
#include <libport/instrument.hh>
#include <libport/read-stdin.hh>

using libport::test_suite;

static void test_read_file()
{
  const std::string input = srcdir() + "/tests/libport/666.txt";
  BOOST_CHECK_EQUAL(libport::read_file(input), "666\n");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::read-stdin test suite");
  suite->add(BOOST_TEST_CASE(test_read_file));
  return suite;
}
