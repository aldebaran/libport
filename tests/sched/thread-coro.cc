/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>

#include <libport/cassert>
#include <libport/cerrno>
#include <libport/pthread.h>
#include <libport/semaphore.hh>
#include <libport/thread-data.hh>
#include <libport/unit-test.hh>

#include <sched/coroutine.hh>

// do not test coroutine with valgrind if it is not enabled.
#if ! defined(USE_VALGRIND) || defined(NVALGRIND)
# include <libport/instrument.hh>

INSTRUMENTFLAGS(--mode=none);
#endif

using libport::test_suite;

void* start0(void*);
void  start1(void*);
void  start2(void*);
void* start3(void*);
void  start4(void*);
void  start5(void*);

static libport::Semaphore out_mutex(1);
static libport::Semaphore s[6];
static Coro* c[6];

// used to synchronized threads.
static libport::Semaphore syn[2];

#define SEM_CURRENT(i)                          \
  int current = i

#define ECHO(S)                                 \
  do {                                          \
    out_mutex--;                                \
    std::cerr << "c[" << current <<  "] "       \
              << S << std::endl;                \
    out_mutex++;                                \
  } while(0)

#define SWITCH_TO(i)                            \
  do {                                          \
    s[(i)]--;                                   \
    ECHO("-> c[" << (i) << "]");                \
    coroutine_switch_to(c[current], c[(i)]);    \
  } while(0)

#define SET_MAIN(ThreadID)                      \
  do {                                          \
    ECHO("start");                              \
    c[current] = coroutine_new();               \
    coroutine_initialize_main(c[current]);      \
  } while(0)

#define SET_SLAVE()                             \
  do {                                          \
    ECHO("start");                              \
  } while(0)

#define LAUNCH(i)                                               \
  do {                                                          \
    ECHO("-> launch c[" << (i) << "]");                         \
    c[i] = coroutine_new();                                     \
    coroutine_start(c[current], c[i], & start ## i, (void*)0);  \
  } while(0)


#define END_SLAVE()                             \
  do {                                          \
    ECHO("End.");                               \
    BOOST_CHECK(false);                         \
  } while(0)

#define END_MAIN()                              \
  do {                                          \
    ECHO("End.");                               \
    BOOST_CHECK(true);                          \
    return 0;                                   \
  } while(0)

// release the semaphore of the origin coroutine and acquire the current
// coroutine.
#define SEM_FROM(f)                             \
  do {                                          \
    s[(f)]++;                                   \
  } while (0)


// The following ASCII art represent thread processes and the coroutine run
// under each processes.
//
//  0   1   2   3   4   5
//
//  |           |
//   ---         ---
//      |           |
//       ---         ---
//          |           |
// -         ---         -
//  |           |
//   ---         ---
//      |  synced   |
// -----     -------     -
//          |           |
//   -------     -------
//  |           |
//
// Coroutine 0 to 2 and 3 to 5 are respectively created by the thread 1 and
// 2.

void* start0(void*)
{
  SEM_CURRENT(0);
  SET_MAIN();

  LAUNCH(1);

  SEM_FROM(5);
  SWITCH_TO(1);

  SEM_FROM(2);
  END_MAIN();
}

void start1(void*)
{
  SEM_CURRENT(1);
  SET_SLAVE();

  SEM_FROM(0);
  LAUNCH(2);

  SEM_FROM(0);
  syn[1]++;
  syn[0]--;
  SWITCH_TO(5);

  END_SLAVE();
}

void start2(void*)
{
  SEM_CURRENT(2);
  SET_SLAVE();

  SEM_FROM(1);
  SWITCH_TO(3);

  SEM_FROM(4);
  SWITCH_TO(0);

  END_SLAVE();
}

void* start3(void*)
{
  SEM_CURRENT(3);
  SET_MAIN();

  LAUNCH(4);

  SEM_FROM(2);
  SWITCH_TO(4);

  SEM_FROM(5);
  END_MAIN();
}

void start4(void*)
{
  SEM_CURRENT(4);
  SET_SLAVE();

  SEM_FROM(3);
  LAUNCH(5);

  SEM_FROM(3);
  syn[1]++;
  syn[0]--;
  SWITCH_TO(2);

  END_SLAVE();
}

void start5(void*)
{
  SEM_CURRENT(5);
  SET_SLAVE();

  SEM_FROM(4);
  SWITCH_TO(0);

  SEM_FROM(1);
  SWITCH_TO(3);

  END_SLAVE();
}

void test_pthread_coro()
{
  pthread_t my_thread[2];
  void* ret;

  BOOST_CHECK(!pthread_create(my_thread    , 0, &start0, 0));
  BOOST_CHECK(!pthread_create(my_thread + 1, 0, &start3, 0));

  syn[1] -= 2;
  syn[0] += 2;

  PTHREAD_RUN(pthread_join, my_thread[0], &ret);
  PTHREAD_RUN(pthread_join, my_thread[1], &ret);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::sched");
  suite->add(BOOST_TEST_CASE(test_pthread_coro));
  return suite;
}
