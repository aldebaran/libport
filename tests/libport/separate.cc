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
 ** Test code for libport/separate.hh features.
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <libport/separate.hh>
#include <libport/pair.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

void
check()
{
# define CHECK(Type, Num, Separator)                    \
  {                                                     \
    std::vector<Type> v(4);                             \
    std::fill(v.begin(), v.end(), Num);			\
    std::ostringstream s;                               \
    s << libport::separate(v, Separator);               \
    BOOST_CHECK_EQUAL(s.str(), "51, 51, 51, 51");       \
  }

  CHECK(int, 51, ", ");
  CHECK(int, 51, libport::make_pair (",", " "));

  int p = 51;
  CHECK(int*, &p, ", ");

  CHECK(const char*, "51", ", ");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::cli");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
