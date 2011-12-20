/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cassert>
#include <libport/cerrno>
#include <libport/foreach.hh>
#include <libport/thread.hh>
#include <libport/unit-test.hh>
#include <libport/synchronizer.hh>

using libport::test_suite;

static libport::Synchronizer s;
static bool mustexit = false;
static std::vector<std::pair<int, int> > data;
typedef std::pair<int, int> pairii;
// bouncer to ease breakpointing
void die()
{
  exit(1);
}
void loop_main()
{
  int ting = 0;
  while(true)
  {
    ting++;
    usleep(20000);
    s.check();
    for(int i=0; i<3; i++)
    {
      foreach(pairii& p, data)
      {
        if (p.first != p.second)
        {
          BOOST_CHECK(!"Data corrupted");
          die();
        }
      }
      usleep(1);
    }
    if (ting == 500)
    {
      mustexit = true;
      usleep(200000);
      s.check();
      usleep(200000);
      s.check();
      return;
    }
  }
}
void loop_task(int i)
{
  int ting = 0;
  while (true)
  {
    usleep(10000 + i*2000);
    {
      libport::Synchronizer::SynchroPoint p(s);
      if (mustexit)
        return;
      data.push_back(std::make_pair(i, i));
      usleep(2000);
      data.back().first = i+5;
      usleep(2000);
      if (data.back().first != i+5)
      {
        BOOST_CHECK(!"Data corrupted");
        die();
      }
      data.back().second = i+5;
      if (data.size() > 50)
        data.pop_back();
    }
    ting++;
  }
}

void test_synchronizer()
{
  BOOST_ASSERT(true);
  for(int i=0; i<10; ++i)
    libport::startThread(boost::bind(&loop_task, i));
  loop_main();
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::synchronizer");
  suite->add(BOOST_TEST_CASE(test_synchronizer));
  return suite;
}
