/*
 * Copyright (C) 2009, Gostai S.A.S.
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
#include <libport/cstdlib>
#include <libport/option-parser.hh>
#include <libport/sysexits.hh>
#include <libport/unit-test.hh>

using libport::test_suite;
using namespace libport;

std::string srcdir = libport::xgetenv("SRCDIR");

void
check_file_contents_get()
{
  const std::string input = srcdir + "/tests/libport/666.txt";
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
check_option_flag()
{
  OptionFlag flag("whether to flag", "flag", 'f');

  OptionParser p;
  p << flag;

  // Test long version
  {
    args_type args;
    args.push_back("--flag");

    p(args);
    BOOST_CHECK(flag.get());
  }

  // Test absence
  {
    args_type args;
    args.push_back("junk");
    args.push_back("junk");
    args.push_back("junk");

    p(args);
    BOOST_CHECK(!flag.get());
  }

  // Test short version
  {
    args_type args;
    args.push_back("junk");
    args.push_back("-f");
    args.push_back("junk");

    p(args);
    BOOST_CHECK(flag.get());
  }
}

void
check_option_combined()
{
  OptionFlag f1("", "flag1", '1');
  OptionFlag f2("", "flag2", '2');
  OptionFlag f3("", "flag3", '3');
  OptionFlag f4("", "flag4", '4');
  OptionFlag f5("", "flag5", '5');

  OptionParser p;
  p << f1 << f2 << f3 << f4 << f5;

  args_type args;
  args.push_back("-135");
  args.push_back("-4");
  p(args);

  BOOST_CHECK(f1.get());
  BOOST_CHECK(!f2.get());
  BOOST_CHECK(f3.get());
  BOOST_CHECK(f4.get());
  BOOST_CHECK(f5.get());
}

void
check_option_end()
{
  OptionsEnd end;

  OptionParser p;
  p << end;

  args_type args;
  args.push_back("1");
  args.push_back("2");
  args.push_back("--");
  args.push_back("3");
  args.push_back("4");
  args = p(args);

  BOOST_CHECK_EQUAL(args.size(), 2u);
  BOOST_CHECK_EQUAL(args[0], "1");
  BOOST_CHECK_EQUAL(args[1], "2");

  BOOST_CHECK_EQUAL(end.get().size(), 2u);
  BOOST_CHECK_EQUAL(end.get()[0], "3");
  BOOST_CHECK_EQUAL(end.get()[1], "4");
}

void
check_option_value()
{
  OptionValue val("set the value", "value", 'v');

  OptionParser p;
  p << val;

  // Test presence
  {
    args_type args;
    args.push_back("junk");
    args.push_back("--value");
    args.push_back("42");
    args.push_back("junk");

    p(args);
    BOOST_CHECK(val.filled());
    BOOST_CHECK_EQUAL(val.value(), "42");
  }

  // Test presence with equal
  {
    args_type args;
    args.push_back("junk");
    args.push_back("--value=51");
    args.push_back("junk");

    p(args);
    BOOST_CHECK(val.filled());
    BOOST_CHECK_EQUAL(val.value(), "51");
  }

  // Test absence
  {
    args_type args;
    args.push_back("junk");
    args.push_back("junk");

    p(args);
    BOOST_CHECK(!val.filled());
  }

  // Test parse error
  {
    args_type args;
    args.push_back("junk");
    args.push_back("--value");

    try
    {
      p(args);
      BOOST_ERROR("Invalid arguments accepted");
    }
    catch (libport::Error& e)
    {
      BOOST_CHECK_EQUAL(e.errors().size(), 1u);
      BOOST_CHECK_EQUAL(e.errors()[0], "--value takes one argument");
    }
  }
}

void
check_program_name()
{
  BOOST_CHECK_EQUAL(libport::program_name(), "test_suite");
}

void
check_usage()
{
  OptionFlag  flag("whether to flag", "flag", 'f');
  OptionValue val ("set the value",   "value", 'v', "VAL");

  OptionParser p;
  p << flag << val;

  {
    std::stringstream s;
    p.usage(s);
    BOOST_CHECK_EQUAL(s.str(),
                      "test_suite [--flag] [--value=VAL]");
  }

  {
    std::stringstream s;
    p.options_doc(s);
    BOOST_CHECK_EQUAL(s.str(),
                      "  -f, --flag       whether to flag\n"
                      "  -v, --value=VAL  set the value\n");
  }
}

test_suite*
init_test_suite()
{
  if (srcdir.empty())
    std::cerr << "SRCDIR is not defined" << std::endl
              << libport::exit(EX_USAGE);

  libport::program_initialize("test_suite");
  test_suite* suite = BOOST_TEST_SUITE("libport::cli");
  suite->add(BOOST_TEST_CASE(check_program_name));
  suite->add(BOOST_TEST_CASE(check_file_contents_get));
  suite->add(BOOST_TEST_CASE(check_option_flag));
  suite->add(BOOST_TEST_CASE(check_option_value));
  suite->add(BOOST_TEST_CASE(check_usage));
  suite->add(BOOST_TEST_CASE(check_option_combined));
  suite->add(BOOST_TEST_CASE(check_option_end));
  return suite;
}
