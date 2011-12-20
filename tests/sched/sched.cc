/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>
#include <sched/coroutine.hh>
#include <tests/libport/test.hh>

// do not test coroutine with valgrind if it is not enabled.
#if ! defined USE_VALGRIND || defined NVALGRIND
# include <libport/instrument.hh>

INSTRUMENTFLAGS(--mode=none);
#endif

using libport::test_suite;

#define ECHO(S)                                 \
  std::cerr << S << std::endl

Coro* mc;
Coro* c1, *c2;

static int step = 0;

static void start2(void*)
{
  int x = 1;
  ECHO("c2 start");
  BOOST_CHECK_EQUAL(step, 3); step++;
  ECHO("c2->c1");
  x++;
  coroutine_switch_to(c2, c1);
  BOOST_CHECK_EQUAL(x, 2);
  x++;
  ECHO("c2->main");
  BOOST_CHECK_EQUAL(step, 8); step++;
  BOOST_CHECK_EQUAL(x, 3);
  coroutine_switch_to(c2, mc);
  ECHO("END!!");
  BOOST_CHECK(false);
}

static void start(void*)
{
  int x = 0;
  ECHO("c1 start");
  BOOST_CHECK_EQUAL(step, 1); step++;
  c2 = coroutine_new();
  ECHO("c1->start c2");
  BOOST_CHECK_EQUAL(step, 2); step++;
  x++;
  coroutine_start(c1, c2, &start2, (void*)0);
  BOOST_CHECK_EQUAL(x, 1);
  x++;
  ECHO("c1->main");
  BOOST_CHECK_EQUAL(step, 4); step++;
  coroutine_switch_to(c1, mc);
  BOOST_CHECK_EQUAL(x, 2);
  x++;
  ECHO("c1->main");
  BOOST_CHECK_EQUAL(step, 6); step++;
  coroutine_switch_to(c1, mc);
  BOOST_CHECK_EQUAL(x, 3);
  x++;
  ECHO("END!!");
  BOOST_CHECK(false);
}

void test_sched()
{
  step = 0;
  mc = coroutine_new();
  coroutine_initialize_main(mc);
  c1 = coroutine_new();
  ECHO("Starting c1");
  BOOST_CHECK_EQUAL(step, 0); step++;
  coroutine_start(mc, c1, &start, (void*)0);
  ECHO("Main->c1");
  BOOST_CHECK_EQUAL(step, 5); step++;
  coroutine_switch_to(mc, c1);
  ECHO("Main->c2");
  BOOST_CHECK_EQUAL(step, 7); step++;
  coroutine_switch_to(mc, c2);
  ECHO("Main done");
  BOOST_CHECK_EQUAL(step, 9); step++;
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::sched");
  suite->add(BOOST_TEST_CASE(test_sched));
  return suite;
}
