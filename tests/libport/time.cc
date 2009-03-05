#include <libport/time.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

static void check_delta(libport::Duration expected,
                        libport::Duration effective,
                        libport::Duration delta)
{
  BOOST_CHECK(effective > expected - delta && effective < expected + delta);
}

static void test_duration()
{
  libport::Duration s = libport::time::s(10);
  libport::Duration ms = libport::time::ms(10);
  libport::Duration us = libport::time::us(10);

  BOOST_CHECK_EQUAL(s, ms * 1000);
  BOOST_CHECK_EQUAL(ms, us * 1000);
  BOOST_CHECK_EQUAL(s - ms, libport::time::ms(9990));
}

static void test_sleep()
{
  libport::Duration wait = libport::time::ms(500);
  libport::Duration delta = libport::time::ms(50);
  libport::Time start = libport::time::now();
  libport::time::sleep(wait);
  libport::Duration elapsed = libport::time::now() - start;
  check_delta(wait, elapsed, delta);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::time test suite");

  suite->add(BOOST_TEST_CASE(test_duration));
  suite->add(BOOST_TEST_CASE(test_sleep));

  return suite;
}
