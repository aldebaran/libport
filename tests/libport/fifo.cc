/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/fifo.hh>
#include <libport/unit-test.hh>

// Use a queue with a small limit to test the
// reorganization.
static libport::Fifo<char, '\0'> queue(8);

// Check size and capacity.
static void
csp(size_t sz, size_t cp)
{
  BOOST_CHECK_EQUAL(queue.size(), sz);
  BOOST_CHECK_EQUAL(queue.capacity(), cp);
}

void t1()
{
  queue.push("Hello");
  csp(5, 8);
}

void t2()
{
  // Check that the peek() doesn't modify the queue.
  BOOST_CHECK(std::string(queue.peek()) == "Hello");
  csp(5, 8);
  BOOST_CHECK(std::string(queue.peek()) == "Hello");
}

void t3()
{
  // Check that consuming the queue works as expected.
  queue.pop(5);
  csp(0, 8);
  BOOST_CHECK(*queue.peek() == '\0');
}

void t4()
{
  // Check that partial retrieval works as well.
  queue.push("Hello");
  csp(5, 8);
  BOOST_CHECK(std::string(queue.peek()) == "Hello");
  BOOST_CHECK(std::string(queue.pop(3), 3) == "Hel");
  csp(2, 8);
  BOOST_CHECK(std::string(queue.peek()) == "lo");
}

void t5()
{
  // Check that we can add data without reallocating.
  queue.push("Hello");
  csp(7, 8);
  BOOST_CHECK(std::string(queue.peek()) == "loHello");
}

void t6()
{
  // Check that we reallocation occurs automatically.
  queue.push(", world, this is me!");
  csp(27, 32);
  BOOST_CHECK(std::string(queue.peek()) == "loHello, world, this is me!");
}

libport::test_suite*
init_test_suite()
{
  libport::test_suite* suite = BOOST_TEST_SUITE("libport::Fifo test suite");

#define ADD_TEST(Purpose, N)						\
  {									\
    libport::test_suite* current = BOOST_TEST_SUITE("Checking " Purpose); \
    suite->add(current);						\
    current->add(BOOST_TEST_CASE(t##N));				\
  }

  ADD_TEST("count", 1);
  ADD_TEST("that peek() doesn't modify the queue", 2);
  ADD_TEST("that consuming the queue works", 3);
  ADD_TEST("that partial retrieval works as well", 4);
  ADD_TEST("that we can add data without reallocating", 5);
  ADD_TEST("that reallocation occurs automatically", 6);

#undef ADD_TEST
  return suite;
}
