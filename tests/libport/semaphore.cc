#include <libport/pthread.h>
#include <libport/semaphore.hh>
#include <libport/unit-test.hh>
#include <libport/unistd.h>
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

void check_timeout()
{
  pthread_t th;
  libport::Semaphore sem(1);
  libport::utime_t t1, t2;

  sem--;
  t1 = libport::utime();
  pthread_create(&th, 0, &thread1, &sem);
  BOOST_CHECK_EQUAL(sem.get(1), false);
  t2 = libport::utime() - t1;
  BOOST_CHECK(t2 < 1500000);
  pthread_join(th, NULL);
  sem++;
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::semaphore");
  suite->add(BOOST_TEST_CASE(test_semaphore));
  suite->add(BOOST_TEST_CASE(check_timeout));
  return suite;
}
