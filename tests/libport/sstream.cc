/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/sstream>
#include <libport/unit-test.hh>

using libport::test_suite;

static void test_stream_wrapper()
{
#define CHECK(Result, Args)                                     \
  BOOST_CHECK_EQUAL((libport::StreamWrapper() << Args).str(),   \
                    Result)
  CHECK("1", 1);
  CHECK("a", "a");
  CHECK("1a", 1 << "a");
  CHECK("1afoo", 1 << "a" << std::string("foo"));
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("Libport.Sstream");
  suite->add(BOOST_TEST_CASE(test_stream_wrapper));
  return suite;
}
