/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#include <libport/debug.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

GD_INIT();
GD_ADD_CATEGORY(TEST);

void
check ()
{
  GD_CATEGORY(TEST);

  GD_FILTER_LOG();

  BOOST_CHECK_EQUAL(GD_CURRENT_LEVEL(), libport::Debug::levels::log);

  GD_FILTER_DEC();
  BOOST_CHECK_EQUAL(GD_CURRENT_LEVEL(), libport::Debug::levels::none);

  BOOST_CHECK(!GD_SHOW_TRACE());

  GD_FILTER_DUMP();
  BOOST_CHECK_EQUAL(GD_CURRENT_LEVEL(), libport::Debug::levels::dump);

  GD_FILTER_INC();
  BOOST_CHECK_EQUAL(GD_CURRENT_LEVEL(), libport::Debug::levels::dump);

  BOOST_CHECK(GD_SHOW_DUMP());

  BOOST_CHECK_NO_THROW(GD_QUIT());
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::debug");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
