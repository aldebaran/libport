/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** Test code for libport/escape.hh.
 */

#include <libport/escape.hh>
#include <libport/cassert>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <libport/unit-test.hh>

using libport::unescape;
using libport::test_suite;

void
check()
{
  std::string u;
# define CHECK(Str1, Unescaped)					\
  do {								\
    u = unescape(Unescaped);					\
    BOOST_CHECK_EQUAL(Str1, u);					\
  } while (0)

  CHECK("\a\b\f\n\r\t\v\\\"", "\\x07\\b\\f\\n\\r\\t\\v\\\\\\\"");
  CHECK("\a", "\\x07");
  CHECK(" ", "\\x20");
}

void
check_except()
{
  std::string except;
# define CHECK_EXCEPT(Str1, Except)				\
  do {								\
    try {							\
      unescape(Str1);						\
    }								\
    catch(std::runtime_error e)					\
    {								\
      except = e.what();					\
    }								\
    BOOST_CHECK_EQUAL(except, Except);				\
  } while (0)

  CHECK_EXCEPT("aaa\\", "invalid escape: '\\' at end of string");
  CHECK_EXCEPT("aaa\\z", "invalid escape: '\\z'");
  CHECK_EXCEPT("aaa\\x1", "invalid escape: '\\x' not followed by two digits");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::unescape");
  suite->add(BOOST_TEST_CASE(check));
  suite->add(BOOST_TEST_CASE(check_except));
  return suite;
}
