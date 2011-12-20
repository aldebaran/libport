/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** Test command line interface tools.
 */

#include <sstream>

#include <libport/cli.hh>
#include "test.hh"

using libport::test_suite;
using namespace libport;

void
check_file_contents_get()
{
  const std::string input = srcdir() + "/tests/libport/666.txt";
# define CHECK(Type, Value)                             \
  do {                                                  \
    Type v = libport::file_contents_get<Type>(input);   \
    BOOST_CHECK_EQUAL(v, Value);                        \
  } while (0)

  CHECK(int, 666);
  CHECK(unsigned, 666u);
  CHECK(std::string, "666");
}

void
check_program_name()
{
  BOOST_CHECK_EQUAL(libport::program_name(), "test_suite");
}

test_suite*
init_test_suite()
{
  libport::program_initialize("test_suite");
  test_suite* suite = BOOST_TEST_SUITE("libport::cli");
  suite->add(BOOST_TEST_CASE(check_program_name));
  suite->add(BOOST_TEST_CASE(check_file_contents_get));
  return suite;
}
