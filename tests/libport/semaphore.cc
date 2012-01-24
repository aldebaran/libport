/*
 * Copyright (C) 2008-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cassert>
#include <libport/cerrno>
#include <libport/pthread.h>
#include <libport/semaphore.hh>
#include <libport/unistd.h>
#include <libport/unit-test.hh>
#include <libport/utime.hh>

using libport::test_suite;

static void test_semaphore()
{
  // The code for the macintosh seems to have a limit that bounds the
  // number of semaphores to about 10000.  Unless we properly reclaim
  // them, of course.
  for (int i = 0; i < 11000; ++i)
    libport::Semaphore(1);
  BOOST_CHECK_EQUAL(0u, libport::Semaphore::instances());
}

void* thread1(void* data)
{
  libport::Semaphore* sem = (libport::Semaphore *)data;
  sleep(2);
  (*sem)++;
  return 0;
}

#if ! defined __APPLE__ && ! defined WIN32
# define CHECK_SEM(N) BOOST_CHECK_EQUAL(sem.value(), N)
#else
# define CHECK_SEM(N) (void)0
#endif

void check_timeout()
{
  libport::Semaphore sem(1);
  CHECK_SEM(1);
  sem--;
  CHECK_SEM(0);
  libport::utime_t t1 = libport::utime();
  pthread_t th;
  PTHREAD_RUN(pthread_create, &th, 0, &thread1, &sem);
  BOOST_CHECK_EQUAL(sem.uget(1000000), false);
  CHECK_SEM(0);
  libport::utime_t t2 = libport::utime() - t1;
  BOOST_CHECK_LT(t2, 1500000);
  PTHREAD_RUN(pthread_join, th, 0);
  CHECK_SEM(1);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::semaphore");
  suite->add(BOOST_TEST_CASE(test_semaphore));
  suite->add(BOOST_TEST_CASE(check_timeout));
  return suite;
}
