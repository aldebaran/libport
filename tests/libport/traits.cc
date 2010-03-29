/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/traits.hh>
#include <libport/unit-test.hh>

using libport::test_suite;
using namespace libport;

void
check_argument()
{
  BOOST_CHECK((meta::Eq<traits::Arg<int>::res, int>::res));
  BOOST_CHECK((meta::Eq<traits::Arg<std::string>::res, const std::string&>::res));
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::traits");
  suite->add(BOOST_TEST_CASE(check_argument));
  return suite;
}
