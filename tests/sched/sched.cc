/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>
#include <sched/coroutine.hh>
#include <libport/unit-test.hh>
#include <libport/instrument.hh>

INSTRUMENTFLAGS(--mode=none);

using libport::test_suite;

#define ECHO(S)                                 \
  std::cerr << S << std::endl

Coro* mc;
Coro* c1, *c2;

static void start2(void*)
{
  int x = 1;
  ECHO("c2 start");
  ECHO("c2->c1");
  x++;
  coroutine_switch_to(c2, c1);
  BOOST_CHECK_EQUAL(x, 2);
  x++;
  ECHO("c2->main");
  BOOST_CHECK_EQUAL(x, 3);
  coroutine_switch_to(c2, mc);
  ECHO("END!!");
}

static void start(void*)
{
  int x = 0;
  ECHO("c1 start");
  c2 = coroutine_new();
  ECHO("c1->start c2");
  x++;
  coroutine_start(c1, c2, &start2, (void*)0);
  BOOST_CHECK_EQUAL(x, 1);
  x++;
  ECHO("c1->main");
  coroutine_switch_to(c1, mc);
  BOOST_CHECK_EQUAL(x, 2);
  x++;
  ECHO("c1->main");
  coroutine_switch_to(c1, mc);
  BOOST_CHECK_EQUAL(x, 3);
  x++;
  ECHO("END!!");
}

void test_sched()
{
  mc = coroutine_new();
  coroutine_initialize_main(mc);
  c1 = coroutine_new();
  ECHO("Starting c1");
  coroutine_start(mc, c1, &start, (void*)0);
  ECHO("Main->c1");
  coroutine_switch_to(mc, c1);
  ECHO("Main->c2");
  coroutine_switch_to(mc, c2);
  ECHO("Main done");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::sched");
  suite->add(BOOST_TEST_CASE(test_sched));
  return suite;
}
