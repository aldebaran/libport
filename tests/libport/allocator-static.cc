/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/allocator-static.hh>
#include <libport/unit-test.hh>

using libport::test_suite;


static int init =
  libport::StaticallyAllocated<4, sizeof(int)>::initialize();

class Int: public libport::StaticallyAllocated<4, sizeof(int)>
{
public:
  Int(int i = 0)
    : i_(i)
  {}

  operator int& ()
  {
    return i_;
  }

  operator const int& () const
  {
    return i_;
  }

  bool operator == (int o) const
  {
    return i_ == o;
  }

private:
  int i_;
};

static void test()
{
  Int* a = new Int(0);
  Int* b = new Int(1);
  Int* c = new Int(2);
  Int* d = new Int(3);

  BOOST_CHECK_EQUAL(*a, 0);
  BOOST_CHECK_EQUAL(*b, 1);
  BOOST_CHECK_EQUAL(*c, 2);
  BOOST_CHECK_EQUAL(*d, 3);

  BOOST_CHECK_THROW(new Int, std::bad_alloc);

  delete b;
  b = new Int(4);

  BOOST_CHECK_EQUAL(*a, 0);
  BOOST_CHECK_EQUAL(*b, 4);
  BOOST_CHECK_EQUAL(*c, 2);
  BOOST_CHECK_EQUAL(*d, 3);

  delete a;
  delete c;

  c = new Int(6);
  a = new Int(5);

  BOOST_CHECK_EQUAL(*a, 5);
  BOOST_CHECK_EQUAL(*b, 4);
  BOOST_CHECK_EQUAL(*c, 6);
  BOOST_CHECK_EQUAL(*d, 3);

  delete a;
  delete b;
  delete c;
  delete d;
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::StaticallyAllocated test suite");
  suite->add(BOOST_TEST_CASE(test));
  return suite;
}
