/*
 * Copyright (C) 2008-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_UNIT_TEST_HH
# define LIBPORT_UNIT_TEST_HH

// See documentation on:
// https://core.gostai.com/projects/common/wiki/BoostUnit

# ifndef BOOST_STATIC
#  define BOOST_TEST_DYN_LINK
# endif

# include <libport/system-warning-push.hh>
#  include <boost/config.hpp>
#  include <boost/test/detail/config.hpp>
#  include <boost/test/unit_test.hpp>
# include <libport/system-warning-pop.hh>

namespace libport
{
  using boost::unit_test::test_suite;
}

// Fwd for the entry point function the user must implement
libport::test_suite*
init_test_suite();

// FIXME: Boost unit-test has a different API depending on whether
// it's static or dynamic (the first defines main, the later doesn't).
// Boost defines BOOST_TEST_DYN_LINK to use the dynamic API, but
// unfortunately it seems it's not available for us to test and switch
// our own API accordingly. For now, we manually define BOOST_STATIC
// on architectures where boost is static. Our m4 macro should define
// it automagically.

# ifndef BOOST_STATIC

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

# else

libport::test_suite* init_unit_test_suite(int, char**)
{
  return init_test_suite();
}

# endif

#endif
