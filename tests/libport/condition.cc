#include <vector>
#include <iostream>
#include <iterator>

#include <libport/condition.hh>
#include <libport/foreach.hh>
#include <libport/unit-test.hh>
#include <libport/thread.hh>
#include <libport/unistd.h>

using namespace libport;

static bool die = false;
static const int NTHREAD = 10;
static const int NITER = 20;
std::vector<int> nCall;
static int targetId = 0;
libport::Condition cond;

namespace std
{
  static ostream&
  operator<<(ostream& o, const vector<int>& ms)
  {
    std::copy(ms.begin(), ms.end(), ostream_iterator<int>(o, " "));
    return o;
  }
}

static void cond_thread(int idx)
{
  while (!die)
  {
    BlockLock bl(cond);
    cond.wait();
    if (targetId == idx || targetId == -1)
      nCall[idx]++;
  }
}

static inline void clear()
{
  nCall.clear();
  nCall.resize(NTHREAD, 0);
}

static void test_condition_broadcast_one()
{
  std::vector<int> expect;
  clear();
  for (int i = 0; i < NTHREAD; i++)
    libport::startThread(boost::bind(&cond_thread, i));

  // Give time to those threads to reach the waiting-on-cond state
  usleep(200000);
  BOOST_TEST_MESSAGE("Singlethread broadcast with specific target");
  for (int i = 0; i < NITER; i++)
    for (int j = 0; j < NTHREAD; j++)
    {
      {
	BlockLock bl(cond);
	targetId = j;
	cond.broadcast();
      }
      // Give time to all threads to execute and wait.
      usleep(100000);
    }
  usleep(200000);

  expect.resize(NTHREAD, NITER);
  BOOST_CHECK_EQUAL(nCall, expect);
}

static void test_condition_broadcast_all()
{
  BOOST_TEST_MESSAGE("Singlethread broadcast without specific target");
  clear();
  targetId = -1;
  for (int i = 0; i < NITER; i++)
  {
    {
      BlockLock bl(cond);
      cond.broadcast();
    }
    //FIXME: a bit bogus, we have no waranty that all listener threads
    // will be back in the wait state, however this is what we test.
    usleep(100000);
  }
  usleep(200000);
  int sumHit = 0;
  foreach(int i, nCall)
    sumHit += i;
  BOOST_TEST_MESSAGE("Distribution: " << nCall);
  BOOST_CHECK_EQUAL(sumHit, NITER * NTHREAD);
}

static void test_condition_signal()
{
  BOOST_TEST_MESSAGE("singlethread signal without specific target");
  clear();
  targetId = -1;
  for (int i = 0; i < NITER * NTHREAD; ++i)
  {
    cond.signal();
    usleep(100000);
  }
  int sumHit = 0;
  foreach(int i, nCall)
    sumHit += i;
  BOOST_TEST_MESSAGE("Distribution: " << nCall);
  BOOST_CHECK_EQUAL(sumHit, NITER * NTHREAD);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::condition");
  suite->add(BOOST_TEST_CASE(test_condition_broadcast_one));
  suite->add(BOOST_TEST_CASE(test_condition_broadcast_all));
  suite->add(BOOST_TEST_CASE(test_condition_signal));
  die = true;
  cond.broadcast();
  return suite;
}
