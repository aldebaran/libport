/**
 ** Test libport::utime.
 */

#include <sstream>

#include <libport/unistd.hh>
#include <libport/unit-test.hh>
#include <libport/utime.hh>

using libport::test_suite;
using namespace libport;

void
check()
{
  BOOST_CHECK_EQUAL(0, libport::utime());
  BOOST_CHECK_LT(0, libport::utime());
  sleep(1);
  BOOST_CHECK_EQUAL(1, libport::utime() / 1000000LL);
}


test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::utime");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
