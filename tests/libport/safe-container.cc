/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <boost/assign/list_of.hpp>
#include <libport/lexical-cast.hh>
#include <libport/cstdlib>

#include <boost/preprocessor.hpp>

#include <libport/containers.hh>
#include <libport/foreach.hh>
#include <libport/program-name.hh>
#include <libport/safe-container.hh>
#include <libport/separate.hh>
#include <libport/sysexits.hh>
#include <libport/time.hh>
#include <libport/unit-test.hh>

using boost::assign::list_of;
using libport::test_suite;

template<typename T>
std::vector<T>
tov(boost::assign_detail::generic_list<T>& l)
{
  return std::vector<T>(l.begin(), l.end());
}

#define tolist(a, b) tov(list_of BOOST_PP_TUPLE_TO_SEQ(a, b))

template<typename T, typename U>
bool
check_list(T& l1, U l2)
{
  typename U::iterator i2 = l2.begin();
  foreach (typename T::value_type& v, l1)
  {
    if (i2 == l2.end())
    {
      BOOST_TEST_MESSAGE("second list too short");
      goto fail;
    }
    if (v != *i2)
    {
      BOOST_TEST_MESSAGE("item differ: " << v << " vs. " << *i2);
      goto fail;
    }
    ++i2;
  }
  if (i2 != l2.end())
  {
    BOOST_TEST_MESSAGE("first list too short");
    goto fail;
  }
  return true;
fail:
  BOOST_TEST_MESSAGE("l1: " << libport::separate(l1, " "));
  BOOST_TEST_MESSAGE("l2: " << libport::separate(l2, " "));
  return false;
}

// Check basic behavior
void test()
{
  libport::SafeContainer<std::list, int> s;
  std::list<int> b = list_of(3)(4)(5)(6)(7)(8)(9)(10)(11);
  foreach(int i, b)
    s.push_back(i);
  s.push_front(2);
  s.push_front(1);
  s.push_back(12);
  s.push_back(13);

  BOOST_CHECK(check_list(s, tolist(13, (1,2,3,4,5,6,7,8,9,10,11,12,13))));
  std::vector<int> tv;
  for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
       i != s.end(); ++i)
  {
    tv.push_back(*i);
    if (*i == 12)
      s.erase(i);
  }
  std::vector<int> expect = tolist(13, (1,2,3,4,5,6,7,8,9,10,11,12,13));
  check_list(tv, expect);

  BOOST_CHECK(check_list(s, tolist(12, (1,2,3,4,5,6,7,8,9,10,11,13))));
  for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
       i != s.end(); ++i)
  {
    if (*i == 9)
      s.erase(std::find(s.begin(), s.end(), 9));
  }

  BOOST_CHECK(check_list(s, tolist(11, (1,2,3,4,5,6,7,8,10,11,13))));
  for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
       i != s.end(); ++i)
  {
    if (*i == 8)
    {
      s.erase(std::find(s.begin(), s.end(), 8));
      s.erase(std::find(s.begin(), s.end(), 10));
      s.erase(std::find(s.begin(), s.end(), 7));
    }
  }
  BOOST_CHECK(check_list(s, tolist(8, (1,2,3,4,5,6,11,13))));
  for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
       i != s.end(); ++i)
  {
    if (*i == 1)
      s.erase(i);
  }
  BOOST_CHECK(check_list(s, tolist(7, (2,3,4,5,6,11,13))));
  tv.clear();
  for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
       i != s.end(); ++i)
  {
    tv.push_back(*i);
    if (*i == 13)
      s.erase(i);
  }
  BOOST_CHECK(check_list(tv, tolist(7, (2,3,4,5,6,11, 13))));
  BOOST_CHECK(check_list(s, tolist(6, (2,3,4,5,6,11))));
}

// Check that everything still works when making multiple turns of the flags.
void test2()
{
  libport::SafeContainer<std::list, int> s;
  std::list<int> b = list_of(3)(4)(5)(6)(7)(8)(9)(10)(11);
  foreach(int i, b)
    s.push_back(i);
  std::vector<int> tv;
  BOOST_CHECK(check_list(s, tolist(9, (3,4,5,6,7,8,9,10,11))));

  // SafeContainers really behave in a specific way when used several
  // times.
  for (int i = 0; i < 100; ++i)
  {
    int sum = 0;
    foreach(int j, s)
      sum += j;
    BOOST_CHECK_EQUAL(sum, 63);
  }
  BOOST_CHECK(check_list(s, tolist(9, (3,4,5,6,7,8,9,10,11))));
  BOOST_CHECK(check_list(s, tolist(9, (3,4,5,6,7,8,9,10,11))));
  for (int i = 0; i < 100; ++i)
  {
    tv.clear();
    for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
         i != s.end(); ++i)
    {
      tv.push_back(*i);
      if (*i == 11) s.erase(i);
    }
    s.push_back(11);
    BOOST_CHECK(check_list(s, tv));
  }
  BOOST_CHECK(check_list(s, tolist(9, (3,4,5,6,7,8,9,10,11))));
  std::vector<int> v(b.begin(), b.end());
  /// keep v and s in sync, randomly remove elements
  for (int i = 0; i < 1000; ++i)
  {
    int item = rand()% v.size();
    int p = 0;
    tv.clear();
    for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
         i != s.end(); ++i, ++p)
    {
      tv.push_back(*i);
      if (p == item) s.erase(i);
    }
    BOOST_CHECK(check_list(v, tv));
    v.erase(v.begin()+item);
    BOOST_CHECK(check_list(s, v));
    v.push_back(rand());
    s.push_back(v.back());
  }

  s.clear();
  foreach(int i, b)
    s.push_back(i);
  BOOST_CHECK_EQUAL(s.size(), 9u);
  std::list<int> ins = list_of(100)(101)(102);
  libport::SafeContainer<std::list, int>::iterator i = s.begin();
  i++;i++;i++;
  s.insert(i, ins.begin(), ins.end());
  BOOST_CHECK(check_list(s, tolist(12, (3,4,5,100,101,102,6,7,8,9,10,11))));
  std::vector<int> iv(s.begin(), s.end());
  BOOST_CHECK(check_list(s, iv));
  BOOST_CHECK(check_list(iv, tolist(12, (3,4,5,100,101,102,6,7,8,9,10,11))));
  s.clear();
  s.insert(s.begin(), iv.begin(), iv.end());
  BOOST_CHECK(check_list(s, tolist(12, (3,4,5,100,101,102,6,7,8,9,10,11))));
  s.clear();
  // Check we can iterate on an empty list.
  foreach(int i, s)
    LIBPORT_USE(i);
}

// Interrupt iteration
void test3()
{
  libport::SafeContainer<std::list, int> s;
  std::vector<int> b =
    list_of(1)(2)(3)(4)(5)(6)(7)(8)(9)(10)(11)(12)(13)(14)(15);
  foreach(int i, b)
    s.push_back(i);
  std::vector<int> tv;
  for (int i = 0; i < 1000; ++i)
  {
    int item = rand() % b.size();
    int p = 0;
    tv.clear();
    for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
         i != s.end(); ++i, ++p)
    {
      if (p == item)
        break;
      tv.push_back(*i);
    }
    BOOST_CHECK(check_list(tv, std::vector<int>(b.begin(), b.begin() + item)));
  }
}

// Check multiple iterators in parallel
void test4()
{
  std::vector<int> b =
    list_of(0)(1)(2)(3)(4)(5)(6)(7)(8)(9)(10)(11)(12)(13)(14)(15);
  std::vector<int> tv;
  for (int i_=0; i_<100; ++i_)
  {
    libport::SafeContainer<std::list, int> s;
    foreach(int i, b)
    s.push_back(i);
    std::vector<libport::SafeContainer<std::list, int>::iterator> iters;
    std::vector<int> ipos;
    std::vector<std::vector<int> > effective;
    // Store some iterators at various positions.
    BOOST_CHECK(true);
    for (int piter=0; piter<10; ++piter)
    {
      BOOST_TEST_MESSAGE("start iteration " << piter);
      effective.push_back(std::vector<int>());
      int item = rand()% b.size();
      int p = 0;
      iters.push_back(s.begin());
      BOOST_CHECK(true);
      libport::SafeContainer<std::list, int>::iterator &i = iters[piter];
      for (; i != s.end(); ++i, ++p)
      {
        effective[piter].push_back(*i);
        if (p == item) break;
      }
      BOOST_CHECK(true);
      ipos.push_back(item);
    }

    // Remove random elements
    std::vector<int> removed;
    for (int i=0; i<5; ++i)
    {
      int item = rand()% b.size();
      int p = 0;
      for (libport::SafeContainer<std::list, int>::iterator i = s.begin();
           i != s.end(); ++i, ++p)
        if (p == item)
        {
          removed.push_back(*i);
          s.erase(i);
          break;
        }
    }

    BOOST_CHECK(true);

    // Finish iteration
    for (int piter=0; piter<10; ++piter)
    {
      BOOST_TEST_MESSAGE("finish iteration " << piter);
      effective[piter].push_back(-1);
      libport::SafeContainer<std::list, int>::iterator &i = iters[piter];
      if (i != s.end())
        i++;
      while (i != s.end())
      {
        effective[piter].push_back(*i);
        ++i;
      }
    }
    BOOST_CHECK(true);
    // Compute expected output and compare
    for (int piter=0; piter<10; ++piter)
    {
      std::vector<int> expect;
      for (int i=0; i<=ipos[piter]; ++i)
        expect.push_back(b[i]);
      expect.push_back(-1);
      for (unsigned i=ipos[piter]+1; i< b.size(); ++i)
        if (!libport::has(removed, b[i]))
          expect.push_back(b[i]);
      BOOST_CHECK(check_list(expect, effective[piter]));
    }
  }
}

test_suite*
init_test_suite()
{
  int seed = lexical_cast<int>(libport::xgetenv("SEED", "0"));
  BOOST_TEST_MESSAGE("seed: "<< seed);
  srand(seed);
  libport::program_initialize("safe-container");
  test_suite* suite = BOOST_TEST_SUITE("libport::SafeContainer test suite");
  suite->add(BOOST_TEST_CASE(test));
  suite->add(BOOST_TEST_CASE(test2));
  suite->add(BOOST_TEST_CASE(test3));
  suite->add(BOOST_TEST_CASE(test4));
  return suite;
}
