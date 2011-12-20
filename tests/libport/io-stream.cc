/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/io-stream.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

static std::string output;
static std::string input = "0123456789";

class TestBuffer: public libport::StreamBuffer
{
  virtual size_t read(char* buffer, size_t max)
  {
    BOOST_CHECK(max > 2);

    if (input.empty())
      return 0;

    buffer[0] = input[0];
    buffer[1] = input[1];
    input = input.substr(2);
    return 2;
  }

  virtual void write(char* buffer, size_t size)
  {
    output += std::string(buffer, size);
  }
};

static void test()
{
  {
    libport::IOStream s(new TestBuffer);

    for (unsigned int i = 0; i < 9; ++i)
    {
      unsigned char c;
      s >> c;
      BOOST_CHECK_EQUAL(c, i + '0');
    }

    for (int i = 9; i >= 0; --i)
      s << (char)(i + '0');
  }
  BOOST_CHECK_EQUAL(output, "9876543210");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::IOStream test suite");
  suite->add(BOOST_TEST_CASE(test));
  return suite;
}
