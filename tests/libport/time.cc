/*
 * Copyright (C) 2009, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
#include <libport/time.hh>
#include <libport/unit-test.hh>
#include <libport/valgrind.hh>

using libport::test_suite;

static void check_delta(libport::Duration expected,
                        libport::Duration effective,
                        libport::Duration delta)
{
  // Does not work: BOOST_CHECK_CLOSE(effective, expected, delta);
  if (RUNNING_ON_VALGRIND)
    delta *= 4;
  BOOST_CHECK_LT(expected - delta, effective);
  BOOST_CHECK_LT(effective, expected + delta);
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
