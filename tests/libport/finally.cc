#include <cassert>
#include <libport/finally.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

void
check()
{
  int var = 1;
  {
    libport::Finally finally(libport::restore(var));
    BOOST_CHECK_EQUAL(var, 1);
    var = 2;
    BOOST_CHECK_EQUAL(var, 2);
  }
  BOOST_CHECK_EQUAL(var, 1);
  {
    libport::Finally finally(libport::scoped_set(var, 2));
    BOOST_CHECK_EQUAL(var, 2);
  }
  BOOST_CHECK_EQUAL(var, 1);
  std::vector<int> v(2, 3);
  {
    libport::Finally finally(libport::scoped_push(4, v));
    BOOST_CHECK_EQUAL(v[2], 4);
    BOOST_CHECK_EQUAL(v.size(), 3u);
  }
  BOOST_CHECK_EQUAL(v.size(), 2u);
  BOOST_CHECK_EQUAL(v[1], 3);

  {
    libport::Finally finally(libport::scoped_push(4, v));
    libport::Finally copy(finally);
    BOOST_CHECK_EQUAL(v[2], 4);
    BOOST_CHECK_EQUAL(v.size(), 3u);
  }
  BOOST_CHECK_EQUAL(v.size(), 2u);
  BOOST_CHECK_EQUAL(v[1], 3);

}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::finally");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
