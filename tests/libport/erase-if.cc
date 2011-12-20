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
#include <libport/containers.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

using namespace libport;

static std::vector<unsigned> v;

static bool
odd(unsigned i)
{
  return i % 2 == 0;
}

void
check ()
{
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  BOOST_CHECK_EQUAL(v.size(), unsigned(5));
  erase_if(v, &odd);
  BOOST_CHECK_EQUAL(v.size(), unsigned(3));
  BOOST_CHECK(libport::has(v, 1));
  BOOST_CHECK(libport::has(v, 3));
  BOOST_CHECK(libport::has(v, 5));
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::erase-if");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
