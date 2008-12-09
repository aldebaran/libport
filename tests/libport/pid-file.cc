#include <fstream>

#include <libport/pid-file.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

void test()
{
  static const libport::path path = "pidfile.pid";
  {
    libport::PidFile f(path.to_string());
    BOOST_CHECK(!path.exists());
    f.create();
    BOOST_CHECK(path.exists());
    std::ifstream input(path.to_string().c_str());
    int pid;
    input >> pid;
    BOOST_CHECK_EQUAL(pid, getpid());
  }
  BOOST_CHECK(!path.exists());
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::PidFile test suite");

  suite->add(BOOST_TEST_CASE(test));

  return suite;
}
