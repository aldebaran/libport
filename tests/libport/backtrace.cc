/**
 ** Test libport::backtrace.
 */

#include <libport/backtrace.hh>
#include <libport/foreach.hh>
#include <libport/unit-test.hh>
#include <libport/compiler.hh>

#include <iostream>

using libport::test_suite;
using namespace libport;

ATTRIBUTE_NOINLINE
void
inner()
{
  std::vector<backtrace_type> bt(backtrace());
  foreach(backtrace_type str, bt)
    std::cout << str  << std::endl;
}

ATTRIBUTE_NOINLINE
void
outer()
{
  inner();
}

void
check()
{
  outer();
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::backtrace");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
