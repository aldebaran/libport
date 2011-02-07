/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#define BUILDING_LIBPORT
# include <libport/singleton-ptr.hh>
#undef BUILDING_LIBPORT
#include <libport/unit-test.hh>

using libport::test_suite;

struct foo
{
  foo()
    : val(0)
  {}

  int bar()
  {
    return val++;
  }
  int val;
};

STATIC_INSTANCE_DECL_NS(foo, f1, /* anonymous namespace */);
STATIC_INSTANCE_DECL_NS(foo, f2, /* anonymous namespace */);

void
check()
{
  BOOST_CHECK_EQUAL(f1->bar(), 0);
  BOOST_CHECK_EQUAL(f1->bar(), 1);
  BOOST_CHECK_EQUAL((*f1).bar(), 2);

  BOOST_CHECK_EQUAL(f2->bar(), 0);
  BOOST_CHECK_EQUAL(f2->bar(), 1);
  BOOST_CHECK_EQUAL((*f2).bar(), 2);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::singleton-ptr test suite");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
