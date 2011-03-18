/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/containers.hh>
#include <libport/debug.hh>
#include <libport/thread.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

GD_INIT();
GD_CATEGORY(TEST);

#ifndef LIBPORT_DEBUG_DISABLE

void
dynamic_level()
{
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

#else

void
dynamic_level()
{
  BOOST_CHECK(true);
}

#endif

static const unsigned concurrent_categories_niter = 4;
static const unsigned concurrent_categories_nthread = 16;

void* concurrent_categories_thread(void*)
{
  for (unsigned i = 0; i < concurrent_categories_niter; ++i)
  {
    GD_INFO("OK.");
  }
  return 0;
}

void
concurrent_categories()
{
  std::vector<pthread_t> threads;

  for (unsigned i = 0; i < concurrent_categories_nthread; ++i)
  {
    pthread_t t;
    pthread_create(&t, 0, concurrent_categories_thread, 0);
    threads << t;
  }
  foreach (const pthread_t& thread, threads)
    pthread_join(thread, 0);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("Libport.Debug");
  suite->add(BOOST_TEST_CASE(dynamic_level));

  // For some spurious reason, this test doesn't work with
  // boost::unit_test. I'm not sure whether it's an actual problem
  // with GD. For now, run it directly.

  // suite->add(BOOST_TEST_CASE(concurrent_categories));
  concurrent_categories();
  return suite;
}
