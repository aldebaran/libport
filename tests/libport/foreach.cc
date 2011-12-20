/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <vector>
#include <sstream>
#include <iostream>
#include <boost/assign.hpp>
using namespace boost::assign;

#include <libport/foreach.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

#define ECHO(S)                                 \
  std::cerr << S << std::endl

void
check ()
{
# define CHECK(Container, Foreach, Expect)	 \
  do {						 \
    Container<int> v = list_of(1)(2)(3)(4);	 \
    std::stringstream o;			 \
    Foreach (int i, v)				 \
      o << i << ',';				 \
    BOOST_CHECK_EQUAL(o.str(), Expect);		 \
  } while (0)

  CHECK(      std::vector,  foreach, "1,2,3,4,");
  CHECK(const std::vector,  foreach, "1,2,3,4,");
  CHECK(      std::vector, rforeach, "4,3,2,1,");
  CHECK(const std::vector, rforeach, "4,3,2,1,");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::foreach");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
