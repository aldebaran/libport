/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/base64.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

static void test()
{
  BOOST_CHECK_EQUAL(libport::base64("gostai:gostai"), "Z29zdGFpOmdvc3RhaQ==");
  BOOST_CHECK_EQUAL(libport::base64("gostai:gosta"),  "Z29zdGFpOmdvc3Rh");
  BOOST_CHECK_EQUAL(libport::base64("gostai:gost"),   "Z29zdGFpOmdvc3Q=");

  unsigned char c[10];
  for (int i = 0; i < 9; ++i)
    c[i] = -i;
  c[9] = 0;
  BOOST_CHECK_EQUAL(libport::base64(std::string((char*) c, 10)),
                    "AP/+/fz7+vn4AA==");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::base64 test suite");
  suite->add(BOOST_TEST_CASE(test));
  return suite;
}
