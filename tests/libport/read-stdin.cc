/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/bind.hh>
#include <libport/fcntl.h>
#include <libport/instrument.hh>
#include <libport/read-stdin.hh>
#include "test.hh"

using libport::test_suite;

static void test_read_file()
{
  const std::string input = srcdir() + "/tests/libport/666.txt";
  BOOST_CHECK_EQUAL(libport::read_file(input), "666\n");
}

static
std::string
file_content(const std::string& input)
{
  int fd = open(input.c_str(), O_RDONLY);
  BOOST_CHECK_NE(fd, -1);
  std::string content;
  try
  {
    while (true)
      content += libport::read_fd(fd);
  }
  catch (...)
  {}
  BOOST_CHECK_NE(close(fd), -1);
  return content;
}

static void test_read_fd(const std::string& input)
{
  std::string reference = libport::read_file(input);
  std::string content = file_content(input);
  BOOST_CHECK_EQUAL(reference.size(), content.size());
  BOOST_CHECK_EQUAL(reference, content);
}

test_suite*
init_test_suite()
{
  std::string short_file = srcdir() + "/tests/libport/666.txt";
  std::string long_file  = "Makefile";

  test_suite* suite = BOOST_TEST_SUITE("libport::read-stdin test suite");
  suite->add(BOOST_TEST_CASE(test_read_file));
  suite->add(BOOST_TEST_CASE(boost::bind(test_read_fd, short_file)));
  // Make sure we can read the same file several times.
  suite->add(BOOST_TEST_CASE(boost::bind(test_read_fd, short_file)));
  suite->add(BOOST_TEST_CASE(boost::bind(test_read_fd, long_file)));
  return suite;
}
