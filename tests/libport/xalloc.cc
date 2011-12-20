/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** Test code for libport/xalloc.hh features.
 */

#include <iostream>
#include <sstream>
#include <libport/xalloc.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

void check_xalloc()
{
  std::ostringstream s;
  int test = 1;
  int test_two = 42;
  int var = 0;

  typedef libport::xalloc<int> int_flag;
  const int_flag flag1, flag2;

  // Initially, should be 0 and 0.
  BOOST_CHECK_EQUAL(flag1(s), 0);
  BOOST_CHECK_EQUAL(flag2(s), 0);

  s << flag1.set(test);
  s << flag1.set(++test);
  // Var should be set to 2.
  var = flag1(s);

  s << flag1.set(++test);
  BOOST_CHECK_EQUAL(var, 2);
  BOOST_CHECK_EQUAL(flag1(s), 3);

  s << flag1.set(++test);
  s << flag1.get(var);
  BOOST_CHECK_EQUAL(var, 4);

  // Let's deal with another xalloc.
  s << flag2.set(test_two);
  // Test if var has not changed.
  BOOST_CHECK_EQUAL(var, 4);

  s << flag2.get(var);
  BOOST_CHECK_EQUAL(var, 42);

  s << flag1.get(var);
  BOOST_CHECK_EQUAL(var, 4);

  // Switch test
  int val = 43;
  s << flag1.swap(val);
  s << flag1.get(var);
  BOOST_CHECK_EQUAL(var, 43);
  s << flag1.swap(val);
  s << flag1.get(var);
  BOOST_CHECK_EQUAL(var, 4);
  // Is it reproducible ?
  ++val;
  s << flag1.swap(val);
  s << flag1.get(var);
  BOOST_CHECK_EQUAL(var, 44);
  s << flag1.swap(val);
  s << flag1.get(var);
  BOOST_CHECK_EQUAL(var, 4);

  BOOST_CHECK_EQUAL(s.str(), "");
}


test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::xalloc");
  suite->add(BOOST_TEST_CASE(check_xalloc));
  return suite;
}
