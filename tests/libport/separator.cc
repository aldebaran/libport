/**
 ** Test code for libport/separator.hh features.
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <libport/contract.hh>
#include <libport/separator.hh>
#include <libport/pair.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

void
check()
{
# define CHECK(Type, Num, Separator)				\
  {								\
    std::vector<Type> v (4);					\
    std::fill (v.begin (), v.end (), Num);			\
    std::ostringstream s;					\
    s << libport::separate (v, Separator);			\
    BOOST_CHECK_EQUAL(s.str (), "51, 51, 51, 51");		\
  }

  CHECK(int, 51, ", ");
  CHECK(int, 51, libport::make_pair (",", " "));

  int p = 51;
  CHECK(int*, &p, ", ");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::cli");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
