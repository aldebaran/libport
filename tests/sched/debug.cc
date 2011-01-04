/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
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

#include <sched/coroutine-data.hh>
#include <sched/coroutine-local-storage.hh>

// do not test coroutine with valgrind if it is not enabled.
#if ! defined USE_VALGRIND || defined NVALGRIND
# include <libport/instrument.hh>

INSTRUMENTFLAGS(--mode=none);
#endif

using libport::test_suite;

static  libport::local_data&
debugger_data_thread_coro_local()
{
  typedef boost::thread_specific_ptr<libport::local_data> thread_storage;
  typedef sched::CoroutineLocalStorage<thread_storage> coro_storage;

  static coro_storage cstorage;

  thread_storage& tstorage = *cstorage;
  if (!tstorage.get())
    tstorage.reset(new libport::local_data);
  return *tstorage;
}

GD_INIT_DEBUG_PER(&debugger_data_thread_coro_local);
GD_CATEGORY(MAIN);

#ifndef LIBPORT_DEBUG_DISABLE

Coro* mc;
Coro* c1;
Coro* c2;

static void start_c1(void*);
static void start_c2(void*);

// check that debug is coroutine safe.
void test_debug()
{
  GD_CATEGORY(MAIN);
  GD_INFO_LOG("Main start");

  mc = coroutine_new();
  coroutine_initialize_main(mc);
  c1 = coroutine_new();

  {
    GD_PUSH("Main indent");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 1u);
    GD_INFO_LOG("Main => C1");
    coroutine_start(mc, c1, &start_c1, (void*)0);
    GD_INFO_LOG("Main <= C1");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 1u);
    GD_INFO_LOG("Main => C1");
    coroutine_switch_to(mc, c1);
    GD_INFO_LOG("Main <= C1");
    coroutine_free(c1);
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 1u);
    GD_INFO_LOG("Main => C2");
    coroutine_switch_to(mc, c2);
    GD_INFO_LOG("Main <= C2");
    coroutine_free(c2);
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 1u);
  }
  BOOST_CHECK_EQUAL(GD_INDENTATION(), 0u);

  GD_INFO_LOG("Main end");
  BOOST_CHECK_NO_THROW(GD_QUIT());
}

static void start_c1(void*)
{
  GD_CATEGORY(C1);
  GD_INFO_LOG("C1 start");
  c2 = coroutine_new();
  BOOST_CHECK_EQUAL(GD_INDENTATION(), 0u);
  GD_INFO_LOG("C1 => C2");
  coroutine_start(c1, c2, &start_c2, (void*)0);
  GD_INFO_LOG("C1 <= C2");
  BOOST_CHECK_EQUAL(GD_INDENTATION(), 0u);

  {
    GD_PUSH("C1 indent");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 1u);
    GD_PUSH("C1 indent");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 2u);
    GD_INFO("C1 => Main");
    coroutine_switch_to(c1, mc);
    GD_INFO("C1 <= Main");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 2u);
  }

  BOOST_CHECK_EQUAL(GD_INDENTATION(), 0u);
  GD_INFO_LOG("C1 end => Main");
  coroutine_switch_to(c1, mc);
}

static void start_c2(void*)
{
  GD_CATEGORY(C2);
  GD_INFO_LOG("C2 start");

  BOOST_CHECK_EQUAL(GD_INDENTATION(), 0u);

  {
    GD_PUSH("C2 indent");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 1u);
    GD_PUSH("C2 indent");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 2u);
    GD_PUSH("C2 indent");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 3u);
    GD_INFO("C2 => C1");
    coroutine_switch_to(c2, c1);
    GD_INFO("C2 <= Main");
    BOOST_CHECK_EQUAL(GD_INDENTATION(), 3u);
    GD_INFO_LOG("C2 end => main");
    coroutine_switch_to(c2, mc);
  }
}

#else

void test_debug()
{
  BOOST_CHECK(true);
}

#endif

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::debug");
  suite->add(BOOST_TEST_CASE(test_debug));
  return suite;
}
