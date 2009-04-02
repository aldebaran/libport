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
  foreach(const std::string& s, backtrace())
    std::cout << s << std::endl;
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
