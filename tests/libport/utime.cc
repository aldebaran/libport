/*
 * Copyright (C) 2009-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** Test libport::utime.
 */

#include <sstream>

#include <libport/unistd.h>
#include <libport/unit-test.hh>
#include <libport/utime.hh>

using libport::test_suite;
using namespace libport;

void
conversions()
{
  BOOST_CHECK_EQUAL(utime(utime_to_timeval(123456789)),
                    123456789);

  // Cannot work with gettimeofday, we overflow utime.
  timeval t = { 1234, 5678 };
  BOOST_CHECK_EQUAL(utime_to_timeval(utime(t)), t);
}

// The static cast is required by msvc's std::abs
#define MY_ABS(T) std::abs(static_cast<long>(T))

void
check_immediate()
{
  libport::utime_t t1 = libport::utime();
  libport::utime_t t2 = libport::utime();
  BOOST_CHECK_LT(t2 - t1, 5000LL);
}

void
check_sleep()
{
  libport::utime_t start = libport::utime();
  sleep(1);
  libport::utime_t end = libport::utime();
  BOOST_CHECK_LT(MY_ABS(end - start - 1000000LL), 20000LL);
}

void
check_reference()
{
  libport::utime_t start = libport::utime();
  libport::utime_reference_set(start);
  sleep(1);
  libport::utime_t end = libport::utime();
  BOOST_CHECK_LT(MY_ABS(end - 1000000LL), 20000LL);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::utime");
  suite->add(BOOST_TEST_CASE(conversions));
  suite->add(BOOST_TEST_CASE(check_immediate));
  suite->add(BOOST_TEST_CASE(check_sleep));
  suite->add(BOOST_TEST_CASE(check_reference));
  return suite;
}
