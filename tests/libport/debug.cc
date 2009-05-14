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
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::debug");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
