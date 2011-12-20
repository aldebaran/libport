/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <vector>
#include <iostream>
#include <iterator>

#include <libport/condition.hh>
#include <libport/foreach.hh>
#include <libport/unit-test.hh>
#include <libport/thread.hh>
#include <libport/unistd.h>

using namespace libport;

static const int NTHREAD = 3;
static const int NITER = 3;
bool die = false;

namespace std
{
  static ostream&
  operator<<(ostream& o, const vector<int>& ms)
  {
    std::copy(ms.begin(), ms.end(), ostream_iterator<int>(o, " "));
    return o;
  }
}

static void cond_thread(int idx,
                        std::vector<int>& output,
                        int& targetId,
                        libport::Condition& cond,
                        libport::Semaphore& sem)
{
  {
    BlockLock bl(cond);
    while (true)
    {
      sem++;
      cond.wait();
      if (targetId == idx || targetId == -1)
        output[idx]++;
      if (die)
        break;
    }
  }
  sem++;
}

static void start_threads(std::vector<int>& output,
                          int& targetId,
                          libport::Condition& cond,
                          libport::Semaphore& sem)
{
  for (int i = 0; i < NTHREAD; i++)
    libport::startThread(boost::bind(&cond_thread, i,
                                     boost::ref(output),
                                     boost::ref(targetId),
                                     boost::ref(cond),
                                     boost::ref(sem)));
  sem -= NTHREAD;
}

static void clear(libport::Condition& cond,
                  libport::Semaphore& sem)
{
  die = true;
  // Lock to ensure all threads are waiting on cond before broadcasting.
  {
    BlockLock bl(cond);
    cond.broadcast();
  }
  sem -= NTHREAD;
  die = false;
}

static void test_condition_broadcast_one()
{
  libport::Condition cond;
  libport::Semaphore sem;
  std::vector<int> output(NTHREAD, 0);
  int tgt = 0;
  start_threads(output, tgt, cond, sem);

  for (int i = 0; i < NITER; i++)
    for (int j = 0; j < NTHREAD; j++)
    {
      {
        BlockLock bl(cond);
        tgt = j;
        cond.broadcast();
      }
      sem -= NTHREAD;
    }

  std::vector<int> expect(NTHREAD, NITER);
  BOOST_CHECK_EQUAL(output, expect);
  clear(cond, sem);
}

static void test_condition_broadcast_all()
{
  libport::Condition cond;
  libport::Semaphore sem;
  std::vector<int> output(NTHREAD, 0);
  int tgt = -1;
  start_threads(output, tgt, cond, sem);

  for (int i = 0; i < NITER; i++)
  {
    {
      BlockLock bl(cond);
      cond.broadcast();
    }
    sem -= NTHREAD;
  }

  int sumHit = 0;
  foreach(int i, output)
    sumHit += i;
  BOOST_TEST_MESSAGE("Distribution: " << output);
  BOOST_CHECK_EQUAL(sumHit, NITER * NTHREAD);
  clear(cond, sem);
}

static void test_condition_signal()
{
  libport::Condition cond;
  libport::Semaphore sem;
  std::vector<int> output(NTHREAD, 0);
  int tgt = -1;
  start_threads(output, tgt, cond, sem);

  for (int i = 0; i < NITER * NTHREAD; ++i)
  {
    cond.signal();
    sem--;
  }

  int sumHit = 0;
  foreach(int i, output)
    sumHit += i;
  BOOST_TEST_MESSAGE("Distribution: " << output);
  BOOST_CHECK_EQUAL(sumHit, NITER * NTHREAD);
  clear(cond, sem);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::condition");
  suite->add(BOOST_TEST_CASE(test_condition_broadcast_one));
  suite->add(BOOST_TEST_CASE(test_condition_broadcast_all));
  suite->add(BOOST_TEST_CASE(test_condition_signal));
  return suite;
}
