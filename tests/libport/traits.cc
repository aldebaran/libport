/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
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

#define BOOST_CHECK_TYPE(T1, T2)                                        \
  BOOST_CHECK_MESSAGE((meta::Eq<T1, T2>::res),                          \
                      (std::string(#T1 " != " #T2 " (")                 \
                       + typeid(T1).name() + " != "                    \
                       + typeid(T2).name() + ")"));                    \


void
check_argument()
{
  BOOST_CHECK_TYPE(traits::RemoveConst<int      >::res, int);
  BOOST_CHECK_TYPE(traits::RemoveConst<const int>::res, int);

  BOOST_CHECK_TYPE(traits::Arg<int>::res, int);
  BOOST_CHECK_TYPE(traits::Arg<std::string>::res, const std::string&);

  BOOST_CHECK_TYPE(traits::Flatten<int>::res, int);
  BOOST_CHECK_TYPE(traits::Flatten<int&>::res, int);
  BOOST_CHECK_TYPE(traits::Flatten<const int>::res, int);
  BOOST_CHECK_TYPE(traits::Flatten<const int&>::res, int);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::traits");
  suite->add(BOOST_TEST_CASE(check_argument));
  return suite;
}
