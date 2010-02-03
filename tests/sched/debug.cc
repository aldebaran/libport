/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
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

using libport::test_suite;

GD_INIT_DEBUG_PER(::libport::localdata::ThreadCoroutine);
GD_ADD_CATEGORY(MAIN);
GD_ADD_CATEGORY(C1);
GD_ADD_CATEGORY(C2);

#define ECHO(S)                                 \
  std::cerr << S << std::endl

Coro* mc;
Coro* c1, *c2;

static void start2(void*)
{
  GD_CATEGORY(C2);
  GD_FILTER_LOG();
  ECHO("(04) c2 start");

  BOOST_CHECK_EQUAL(GD_CURRENT_LEVEL(), libport::Debug::levels::log);
  GD_FILTER_DEC();

  ECHO("(05) c2->c1");
  coroutine_switch_to(c2, c1);

  // should not be libport::Debug::levels::dump
  BOOST_CHECK_EQUAL(GD_CURRENT_LEVEL(), libport::Debug::levels::none);
  BOOST_CHECK(!GD_SHOW_TRACE());

  ECHO("(10) c2->main");
  coroutine_switch_to(c2, mc);

  ECHO("(!!) END!!");
}

static void start(void*)
{
  GD_CATEGORY(C1);
  GD_FILTER_DUMP();
  ECHO("(02) c1 start");
  c2 = coroutine_new();

  BOOST_CHECK_EQUAL(GD_CURRENT_LEVEL(), libport::Debug::levels::dump);
  GD_FILTER_INC();

  ECHO("(03) c1->start c2");
  coroutine_start(c1, c2, &start2, (void*)0);

  // should not be libport::Debug::levels::none
  BOOST_CHECK_EQUAL(GD_CURRENT_LEVEL(), libport::Debug::levels::dump);
  BOOST_CHECK(GD_SHOW_DUMP());

  ECHO("(06) c1->main");
  coroutine_switch_to(c1, mc);

  ECHO("(08) c1->main");
  coroutine_switch_to(c1, mc);

  ECHO("(!!) END!!");
}

// check that debug is coroutine safe.
void test_debug()
{
  GD_CATEGORY(MAIN);

  ECHO("(00) Main start");

  mc = coroutine_new();
  coroutine_initialize_main(mc);
  c1 = coroutine_new();

  ECHO("(01) Main->start c1");
  coroutine_start(mc, c1, &start, (void*)0);

  ECHO("(07) Main->c1");
  coroutine_switch_to(mc, c1);

  ECHO("(09) Main->c2");
  coroutine_switch_to(mc, c2);

  ECHO("(11) Main done");
  BOOST_CHECK_NO_THROW(GD_QUIT());
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::debug");
  suite->add(BOOST_TEST_CASE(test_debug));
  return suite;
}
