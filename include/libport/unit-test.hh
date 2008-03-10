#ifndef LIBPORT_UNIT_TEST_HH
# define LIBPORT_UNIT_TEST_HH

// See documentation on:
// https://core.gostai.com/projects/common/wiki/BoostUnit

# define BOOST_TEST_DYN_LINK
# include <boost/test/unit_test.hpp>

namespace libport
{
  using boost::unit_test::test_suite;
}

// Fwd for the entry point function the user must implement
libport::test_suite*
init_test_suite();

namespace libport
{
  bool _init_test_suite()
  {
    boost::unit_test::framework::master_test_suite().add(init_test_suite());
    return true;
  }
}

int
main(int argc, char** argv)
{
  return ::boost::unit_test::unit_test_main(
    libport::_init_test_suite, argc, argv);
}

#endif
