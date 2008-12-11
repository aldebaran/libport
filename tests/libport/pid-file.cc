#include <fstream>

#include <libport/pid-file.hh>
#include <libport/program-name.hh>
#include <libport/unit-test.hh>
#include <libport/unistd.h>

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

void test_options()
{
  static const char* file = "opt.pid";
  const char* argv[] = {"./name", "--opt", "val", "--pid-file", file, "file1", "file2"};
  int old_argc = sizeof(argv) / sizeof(char*);
  int argc = old_argc;
  static const libport::path path(file);
  {
    libport::PidFile f(argc, argv);
    BOOST_CHECK(!path.exists());
    f.create();
    BOOST_CHECK(path.exists());
    std::ifstream input(path.to_string().c_str());
    int pid;
    input >> pid;
    BOOST_CHECK_EQUAL(pid, getpid());
  }
  BOOST_CHECK(!path.exists());
  BOOST_CHECK_EQUAL(argc, old_argc - 2);
}

test_suite*
init_test_suite()
{
  libport::program_name = "pid-file";
  test_suite* suite = BOOST_TEST_SUITE("libport::PidFile test suite");

  suite->add(BOOST_TEST_CASE(test));
  suite->add(BOOST_TEST_CASE(test_options));

  return suite;
}
