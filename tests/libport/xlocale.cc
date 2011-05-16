/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/locale.hh>
#include <libport/xlocale.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

// Check that we are correctly robust to using LC_NUMERIC using "," in
// numbers.
static void test_strto_c()
{
  try
  {
    std::string loc = libport::setlocale("LC_ALL", "fr_FR");
    BOOST_CHECK(loc != "");
  }
  catch (const std::runtime_error& e)
  {
    BOOST_TEST_MESSAGE(std::string("cannot run locale tests: ") + e.what());
    return;
  }
  BOOST_CHECK_EQUAL(libport::strtof_c("12.3", 0), 12.3f);
  BOOST_CHECK_EQUAL(libport::strtod_c("12.3", 0), 12.3);
  BOOST_CHECK_EQUAL(libport::strto_c<float>("12.3", 0), 12.3f);
  BOOST_CHECK_EQUAL(libport::strto_c<double>("12.3", 0), 12.3);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("Libport.Xlocale");
  suite->add(BOOST_TEST_CASE(test_strto_c));
  return suite;
}
