#include <libport/base64.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

static void test()
{
  BOOST_CHECK_EQUAL(libport::base64("gostai:gostai"), "Z29zdGFpOmdvc3RhaQ==");
  BOOST_CHECK_EQUAL(libport::base64("gostai:gosta"),  "Z29zdGFpOmdvc3Rh");
  BOOST_CHECK_EQUAL(libport::base64("gostai:gost"),   "Z29zdGFpOmdvc3Q=");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::base64 test suite");

  suite->add(BOOST_TEST_CASE(test));

  return suite;
}
