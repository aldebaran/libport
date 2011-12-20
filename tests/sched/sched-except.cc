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
static size_t STACK_SIZE = 0;
#define ECHO(S)                                 \
  std::cerr << S << std::endl

#define CHECK_EXCEPTION(where)                  \
do {                                            \
  ECHO("Checking exception in " #where);        \
  int x = 0;                                    \
  try {                                         \
    x++;                                        \
    throw 15;                                   \
    x--;                                        \
  }                                             \
  catch(int i)                                  \
  {                                             \
    ECHO("Caught exception in " #where);        \
    x++;                                        \
  }                                             \
  BOOST_CHECK_EQUAL(x, 2);                      \
} while(false)                                  \

Coro* mc;
Coro* c1, *c2;

static void start2(void*)
{
  int x = 1;
  ECHO("c2 start");
  CHECK_EXCEPTION(c2);
  ECHO("c2->c1");
  x++;
  coroutine_switch_to(c2, c1);
  BOOST_CHECK_EQUAL(x, 2);
  x++;
  CHECK_EXCEPTION(c2);
  ECHO("c2->main");
  BOOST_CHECK_EQUAL(x, 3);
  coroutine_switch_to(c2, mc);
  CHECK_EXCEPTION(c2);
  ECHO("END!!");
}

static void start(void*)
{
  int x = 0;
  ECHO("c1 start");
  CHECK_EXCEPTION(c1);

  c2 = coroutine_new(STACK_SIZE);
  ECHO("c1->start c2");
  x++;
  coroutine_start(c1, c2, &start2, (void*)0);
  BOOST_CHECK_EQUAL(x, 1);
  x++;
  //CHECK_EXCEPTION(c1);

  ECHO("c1->main");
  coroutine_switch_to(c1, mc);
  //BOOST_CHECK_EQUAL(x, 2);
  x++;
  CHECK_EXCEPTION(c1);
  ECHO("c1->main");
  coroutine_switch_to(c1, mc);
  //BOOST_CHECK_EQUAL(x, 3);
  x++;
  ECHO("END!!");
}

void test_sched()
{
  CHECK_EXCEPTION(main);
  mc = coroutine_new(STACK_SIZE);
  coroutine_initialize_main(mc);
  CHECK_EXCEPTION(main);
  c1 = coroutine_new(STACK_SIZE);
  ECHO("Starting c1");
  coroutine_start(mc, c1, &start, (void*)0);
  CHECK_EXCEPTION(main);
  ECHO("Main->c1");
  coroutine_switch_to(mc, c1);
  CHECK_EXCEPTION(main);
  ECHO("Main->c2");
  coroutine_switch_to(mc, c2);
  ECHO("Main done");
}

test_suite*
init_test_suite()
{
  char* ssize = getenv("STACK_SIZE");
  if (ssize)
    STACK_SIZE = strtol(ssize, 0,0);
  test_suite* suite = BOOST_TEST_SUITE("libport::sched");
  suite->add(BOOST_TEST_CASE(test_sched));
  return suite;
}
