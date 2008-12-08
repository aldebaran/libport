#include <libport/semaphore.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

static void test_semaphore()
{
  // The code for the macintosh seems to have a limit that bounds the
  // number of semaphores to about 10000.  Unless we properly reclaim
  // them, of course.
  for (int i = 0; i < 11000; ++i)
    libport::Semaphore(1);
  BOOST_CHECK_EQUAL(0, libport::Semaphore::instances());
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::semaphore");
  suite->add(BOOST_TEST_CASE(test_semaphore));
  return suite;
}
