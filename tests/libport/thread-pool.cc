/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/bind.hh>
#include <libport/lexical-cast.hh>
#include "test.hh"

#include <libport/unistd.h>
#include <libport/thread-pool.hh>

// For atomic increment
#include <boost/interprocess/detail/atomic.hpp>

#include <boost/version.hpp>
namespace ipcdetail =
#if BOOST_VERSION < 104800
   boost::interprocess::detail;
#else
   boost::interprocess::ipcdetail;
#endif

using ipcdetail::atomic_inc32;
using ipcdetail::atomic_read32;


using libport::test_suite;

typedef libport::ThreadPool ThreadPool;

/* Determinism warning: Do not use rand outside main thread!
*/
volatile boost::uint32_t counter;

// Divisor for number of iterations
static boost::uint32_t dfactor = 1;

static void task_sleep_inc(int delay)
{
  usleep(delay);
  atomic_inc32(&counter);
}

// Just start a bunch of tasks and ensure they are all executed.
// In slowInject, inject slower to trigger the IDLE thread code more often.
static void test_many(bool slowInject)
{
  ThreadPool& tp = *new ThreadPool(10);
  counter = 0;
  std::vector<ThreadPool::rTaskLock> v;
  for(int i=0; i<10; ++i)
    v.push_back(new ThreadPool::TaskLock);
  // Start many tasks with random delay and lock
  static const boost::uint32_t nTasks = 8000 / dfactor;
  for (unsigned i=0; i<nTasks; ++i)
  {
    ThreadPool::rTaskLock lock;
    long delay = rand() % 5000;
    if (rand()%100 > 20)
      lock = v[rand()%v.size()];
    tp.queueTask(boost::bind(&task_sleep_inc, delay), lock);
    usleep(rand()%(slowInject?5000:50));
  }
  // Give it 10s to finish, check regularly.
  boost::uint32_t val;
  for (int i=0; i<20; ++i)
  {
    val = atomic_read32(&counter);
    if (val == nTasks)
      break;
    usleep(500000);
  }
  BOOST_CHECK_EQUAL(nTasks, val);
}

static void test_many_slow()
{
  test_many(true);
}

static void test_many_fast()
{
  test_many(false);
}

std::vector<unsigned> lockCheck;

boost::uint32_t errors = 0;

static void task_sleep_check_lock(int delay, unsigned lockid, unsigned lockval)
{
  lockCheck[lockid] = lockval;
  for (int i=0; i<10; ++i)
  {
    usleep(delay/10);
    if (lockCheck[lockid] != lockval)
      atomic_inc32(&errors);
    lockCheck[lockid] = lockval;
  }
  atomic_inc32(&counter);
}

// Test that no two tasks with same lock are executed in parallel.
static void test_lock()
{
  ThreadPool& tp = *new ThreadPool(10);
  counter = 0;
  std::vector<ThreadPool::rTaskLock> v;
  for(int i=0; i<10; ++i)
    v.push_back(new ThreadPool::TaskLock);
  lockCheck.resize(v.size());
  static const boost::uint32_t nTasks = 4000 / (dfactor * ((dfactor!=1)+1));
  for (unsigned i=0; i<nTasks; ++i)
  {
    unsigned lockid = rand()%v.size();
    long delay = rand() % 5000;
    tp.queueTask(boost::bind(&task_sleep_check_lock, delay, lockid, i),
                 v[lockid]);
  }
  boost::uint32_t val;
  for (int i=0; i<40; ++i)
  {
    val = atomic_read32(&counter);
    if (val == nTasks)
      break;
    usleep(500000);
  }
  BOOST_CHECK_EQUAL(nTasks, val);
  BOOST_CHECK_EQUAL(errors, 0U);
}

// Test the dropping feature
static void test_drop()
{
  ThreadPool& tp = *new ThreadPool(10);
  ThreadPool::rTaskLock l(new ThreadPool::TaskLock(1));
  ThreadPool::rTaskHandle h1 = tp.queueTask(boost::bind(&usleep, 200000), l);
  ThreadPool::rTaskHandle h2 = tp.queueTask(boost::bind(&usleep, 200000), l);
  usleep(100000);
  ThreadPool::rTaskHandle h3 = tp.queueTask(boost::bind(&usleep, 200000), l);
  usleep(300000);
  BOOST_CHECK_EQUAL(h1->getState(), ThreadPool::TaskHandle::FINISHED);
  BOOST_CHECK_EQUAL(h2->getState(), ThreadPool::TaskHandle::DROPPED);
  BOOST_CHECK_EQUAL(h3->getState(), ThreadPool::TaskHandle::DROPPED);
}

test_suite*
init_test_suite()
{
  unsigned int seed = time(0);
  if (char * sseed = getenv("RAND_SEED"))
    seed = boost::lexical_cast<unsigned int>(sseed);
  test_suite* suite = BOOST_TEST_SUITE("libport::ThreadPool test suite");
  BOOST_TEST_MESSAGE("Seed is " << seed);
  if (running("windows"))
    dfactor = 10;
  srand(seed);
  suite->add(BOOST_TEST_CASE(test_many_slow));
  suite->add(BOOST_TEST_CASE(test_many_fast));
  suite->add(BOOST_TEST_CASE(test_lock));
  suite->add(BOOST_TEST_CASE(test_drop));
  return suite;
}
