/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/detect-win32.h>
#include <libport/dirent.h>
#include <libport/unit-test.hh>

using libport::test_suite;

static const std::string myself = "dirent" EXEEXT;

static void test()
{
  DIR* dir = opendir("tests/libport");
  struct dirent* entry;
  bool found = false;

  while ((entry = readdir(dir)))
    found = found || myself == entry->d_name;

  BOOST_CHECK(found);

  closedir(dir);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::dirent");
  suite->add(BOOST_TEST_CASE(test));
  return suite;
}
