/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstdlib>
#include <libport/unit-test.hh>
#include <libport/instrument.hh>

// For some reason, on the Mac, using Valgrind, system lets SIGCHLD
// pass through, which results in a failure.
//
//   Entering test case "test_system"
//   --86515:0:syswrap- WARNING: Ignoring sigreturn( ..., UC_RESET_ALT_STACK );
//   ==86515== Warning: client switching stacks?
//                      SP change: 0x685940 --> 0xbffff400
//   ==86515==          to suppress, use: --max-stackframe=1080583488 or greater
//   unknown location:0: fatal error in "test_system":
//       signal: SIGCHLD,
//       si_code: 0 (child process has terminated; pid: 0; uid: 0; exit value: 0)
//   ../source/tests/libport/cstdlib.cc:43: last checkpoint
//   Leaving test case "test_system"; testing time: 246259mks
//
// And since Boost.Test is no malloc-clean, we really need to disable
// the checks here.
#ifdef __APPLE__
INSTRUMENTFLAGS(--mode=none);
#endif

using libport::test_suite;

static void test_environ()
{
  // Variables we know to be defined.
  const char* path = libport::getenv("PATH");
  BOOST_CHECK(!!path);
  BOOST_CHECK(!!libport::getenv("SRCDIR"));

  // setenv should not override by default.
  BOOST_CHECK(!setenv("PATH", "TOTO", 0));
  BOOST_CHECK_EQUAL(libport::getenv("PATH"), path);

  // Variables we know (hope) not to be defined.
  const char* var = "__THIS_IS_AN_UNLIKELY_VARIABLE_NAME__";
  BOOST_CHECK(!libport::getenv(var));
  BOOST_CHECK(!setenv(var, "23", 0));
  BOOST_CHECK_EQUAL("23", libport::getenv(var));
  BOOST_CHECK(!unsetenv(var));
  BOOST_CHECK(!libport::getenv(var));
}

// Testing system under windows is a pain. We're probably not actually
// going to use it anyway ...
#ifndef WIN32
static void test_system()
{
  // Unfortunately, boost::unit_test implodes if any child has a non null
  // return value, so we cannot test the failing case.

  BOOST_CHECK_EQUAL(libport::system("true"), 0);
  // BOOST_CHECK_EQUAL(libport::system("false"), 1);
  BOOST_CHECK_NO_THROW(int ignored = libport::system("true"); (void)ignored);
  // BOOST_CHECK_THROW(int ignored = libport::system("false"); (void)ignored,
  //                   std::exception);
}
#endif

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::cstdlib");
  suite->add(BOOST_TEST_CASE(test_environ));

#ifndef WIN32
  suite->add(BOOST_TEST_CASE(test_system));
#endif
  return suite;
}
