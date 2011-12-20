/*
 * Copyright (C) 2007-2011, Gostai S.A.S.
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

#include <sstream>
#include <libport/escape.hh>
#include <libport/unit-test.hh>
#include <libport/lexical-cast.hh>

using libport::escape;
using libport::test_suite;

void
check()
{
# define CHECK(In, Out)                         \
  do {                                          \
    std::ostringstream o;                       \
    o << escape In;                             \
    BOOST_CHECK_EQUAL(o.str(), Out);            \
  } while (0)

  CHECK(("\a\b\f\n\r\t\v\\\""), "\\x07\\b\\f\\n\\r\\t\\v\\\\\\\"");

  CHECK(("\""), "\\\"");
  CHECK(("'"),  "'");
  CHECK((""),   "");

  CHECK(("'",  '\''), "\\\'");
  CHECK(("\"", '\''), "\"");
# undef CHECK
}


#include <boost/version.hpp>

void
check_lexical_cast()
{
# define CHECK(In, Out)                                 \
  BOOST_CHECK_EQUAL(string_cast(escape In), Out)

  CHECK(("\""), "\\\"");
  CHECK(("'"),  "'");
  // This does not work in Boost 1.48, see
  // https://svn.boost.org/trac/boost/ticket/6132.
#if BOOST_VERSION != 104800
  CHECK((""),   "");
#endif
# undef CHECK
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::escape");
  suite->add(BOOST_TEST_CASE(check));
  suite->add(BOOST_TEST_CASE(check_lexical_cast));
  return suite;
}
