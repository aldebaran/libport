/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/reserved-vector.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

#define DISTANCE (std::abs(reinterpret_cast<char*>(&v) -        \
                           reinterpret_cast<char*>(&v[0])))

// A value between the size of an std::vector (without the allocator)
// and the distance between the stack and the heap (in bytes).
#define VECTOR_BASE_SIZE 64

static void test_stack()
{
  libport::ReservedVector<int, 4> v;

  v.push_back(0);
  // Check we are on the stack
  BOOST_CHECK_LE(DISTANCE, VECTOR_BASE_SIZE);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  // Check we are still on the stack
  BOOST_CHECK_LE(DISTANCE, VECTOR_BASE_SIZE);
  v.push_back(4);
  // Check we are on the heap
  BOOST_CHECK_GT(DISTANCE, VECTOR_BASE_SIZE);
  v.pop_back();
  v.pop_back();
  v.pop_back();
  v.pop_back();
  v.pop_back();
}

static void test_copy()
{
  libport::ReservedVector<int, 4> src;

  src.push_back(0);
  src.push_back(1);
  src.push_back(2);
  src.push_back(3);
  {
    libport::ReservedVector<int, 4> v(src);
    BOOST_CHECK_LE(DISTANCE, VECTOR_BASE_SIZE);
  }
  {
    libport::ReservedVector<int, 4> v;
    v = src;
    BOOST_CHECK_LE(DISTANCE, VECTOR_BASE_SIZE);
  }
  {
    libport::ReservedVector<int, 4> v(src.begin(), src.end());
    BOOST_CHECK_LE(DISTANCE, VECTOR_BASE_SIZE);
  }
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::ReservedVector test suite");
  suite->add(BOOST_TEST_CASE(test_stack));
  suite->add(BOOST_TEST_CASE(test_copy));
  return suite;
}
