/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/config.h>
#include <libport/base64.hh>
#include <libport/hmac-sha1.hh>
#include <libport/unit-test.hh>

using libport::test_suite;
#include <iostream>
static void test()
{
#ifdef LIBPORT_ENABLE_SSL
  std::string msg =
    "GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n"
    "/johnsmith/photos/puppy.jpg";
  std::string key = "uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o";
  BOOST_CHECK_EQUAL(libport::base64(libport::hmac_sha1(msg, key)),
                    "xXjDGYUmKxnwqr5KXNPGldn5LbA=");
#endif
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::hmac_sha1 test suite");
  suite->add(BOOST_TEST_CASE(test));
  return suite;
}
