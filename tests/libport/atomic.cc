/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <set>

#include <libport/atomic.hh>
#include <libport/containers.hh>
#include <libport/thread.hh>
#include <libport/unit-test.hh>
#include <libport/windows.hh>

using libport::test_suite;

static void increment(long* ptr, long times)
{
  for (long i = 0; i < times; ++i)
    libport::atomic::increment_fetch(ptr);
}

static void decrement(long* ptr, long times)
{
  for (long i = 0; i < times; ++i)
    libport::atomic::decrement_fetch(ptr);
}

static void increment_decrement()
{
  long val = 0;

  pthread_t inc =
    libport::startThread(boost::bind(increment, &val, 1024 * 1024 * 2));
  pthread_t dec =
    libport::startThread(boost::bind(decrement, &val, 1024 * 1024));
  pthread_join(inc, 0);
  pthread_join(dec, 0);

  BOOST_CHECK_EQUAL(val, 1024 * 1024);
}

#define TEST(Fun, Start, Comp, Step)                                    \
  static void Fun##_check_thread(long* ptr, std::set<long>* out,	\
                                 long times)				\
  {                                                                     \
    for (long i = 0; i < times; ++i)					\
      out->insert(libport::atomic::Fun(ptr));                           \
  }                                                                     \
                                                                        \
  static void Fun##_check()                                             \
  {                                                                     \
    std::set<long> set1;						\
    std::set<long> set2;						\
    long val = 0;							\
    static const long max = 1024;					\
                                                                        \
    pthread_t inc1 =                                                    \
      libport::startThread(boost::bind(Fun##_check_thread,              \
                                       &val, &set1, max));              \
    pthread_t inc2 =                                                    \
      libport::startThread(boost::bind(Fun##_check_thread,              \
                                       &val, &set2, max));              \
    pthread_join(inc1, 0);                                              \
    pthread_join(inc2, 0);                                              \
                                                                        \
    for (long i = Start; i Comp 2 * max; Step i)			\
      BOOST_CHECK(libport::mhas(set1, i) ^ libport::mhas(set2, i));     \
  }                                                                     \

TEST(increment_fetch,  1, <=  , ++);
TEST(fetch_increment,  0, <   , ++);
TEST(decrement_fetch, -1, >= -, --);
TEST(fetch_decrement,  0, >  -, --);

#undef TEST

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::atomic test suite");
  suite->add(BOOST_TEST_CASE(increment_decrement));
  suite->add(BOOST_TEST_CASE(increment_fetch_check));
  suite->add(BOOST_TEST_CASE(fetch_increment_check));
  suite->add(BOOST_TEST_CASE(decrement_fetch_check));
  suite->add(BOOST_TEST_CASE(fetch_decrement_check));
  return suite;
}
