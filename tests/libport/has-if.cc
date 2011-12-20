/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstdlib>
#include <vector>
#include <boost/lambda/lambda.hpp>
#include <libport/containers.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

static std::vector<int> v;

void
check ()
{
  v.push_back(1);
  v.push_back(2);

# define CHECK(Num, Bool)						\
  do {									\
    BOOST_CHECK_EQUAL(Bool,                                             \
                      libport::has_if(v, boost::lambda::_1 == Num));	\
  } while (0)

  CHECK(1, true);
  CHECK(2, true);
  CHECK(3, false);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::has-if");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
