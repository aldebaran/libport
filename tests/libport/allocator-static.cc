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

class Int: public libport::StaticallyAllocated<Int, 4>
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

  static const size_t allocator_static_max_size;

private:
  int i_;
};

const size_t Int::allocator_static_max_size = sizeof(Int);

static void test()
{
  Int* a = new Int(0);
  Int* b = new Int(1);
  Int* c = new Int(2);
  Int* d = new Int(3);
  Int* e = new Int(4);
  Int* f = new Int(5);
  Int* g = new Int(6);
  Int* h = new Int(7);

  BOOST_CHECK_EQUAL(*a, 0);
  BOOST_CHECK_EQUAL(*b, 1);
  BOOST_CHECK_EQUAL(*c, 2);
  BOOST_CHECK_EQUAL(*d, 3);
  BOOST_CHECK_EQUAL(*e, 4);
  BOOST_CHECK_EQUAL(*f, 5);
  BOOST_CHECK_EQUAL(*g, 6);
  BOOST_CHECK_EQUAL(*h, 7);

  BOOST_CHECK_NO_THROW(delete b);
  BOOST_CHECK_NO_THROW(b = new Int(8));

  BOOST_CHECK_EQUAL(*a, 0);
  BOOST_CHECK_EQUAL(*b, 8);
  BOOST_CHECK_EQUAL(*c, 2);
  BOOST_CHECK_EQUAL(*d, 3);
  BOOST_CHECK_EQUAL(*e, 4);
  BOOST_CHECK_EQUAL(*f, 5);
  BOOST_CHECK_EQUAL(*g, 6);
  BOOST_CHECK_EQUAL(*h, 7);

  delete a;
  delete c;

  c = new Int(9);
  a = new Int(10);

  BOOST_CHECK_EQUAL(*a, 10);
  BOOST_CHECK_EQUAL(*b, 8);
  BOOST_CHECK_EQUAL(*c, 9);
  BOOST_CHECK_EQUAL(*d, 3);
  BOOST_CHECK_EQUAL(*e, 4);
  BOOST_CHECK_EQUAL(*f, 5);
  BOOST_CHECK_EQUAL(*g, 6);
  BOOST_CHECK_EQUAL(*h, 7);

  delete a;
  delete b;
  delete c;
  delete d;
  delete e;
  delete f;
  delete g;
  delete h;
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::StaticallyAllocated test suite");
  suite->add(BOOST_TEST_CASE(test));
  return suite;
}
