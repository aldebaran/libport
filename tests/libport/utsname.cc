/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <sstream>
#include <libport/sys/utsname.h>
#include <libport/unit-test.hh>

using libport::test_suite;
using namespace libport;

void
check_names()
{
  libport::utsname names;
#define CHECK(Routine)                                          \
  do {                                                          \
    BOOST_CHECK_NO_THROW(names.Routine());                      \
    BOOST_TEST_MESSAGE(#Routine ": " << names.Routine());       \
  } while (false)
  CHECK(machine);
  CHECK(node);
  CHECK(release);
  CHECK(release_major);
  CHECK(release_minor);
  CHECK(release_patchlevel);
  CHECK(system);
  CHECK(version);
#undef CHECK
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE(__FILE__);
  suite->add(BOOST_TEST_CASE(check_names));
  return suite;
}
