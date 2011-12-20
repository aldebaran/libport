/*
 * Copyright (C) 2009-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** Test -e/-f support.
 */

#include <sstream>

#include <libport/input-arguments.hh>
#include <libport/containers.hh>
#include <libport/separate.hh>
#include "test.hh"

using libport::test_suite;
using namespace libport;

// A cl.exe bug prevents the definition of DataCollector in a
// function.
struct DataCollector : libport::opts::DataVisitor
{
  typedef libport::opts::DataVisitor super_type;
  using super_type::operator();

  void
  operator()(const libport::opts::TextData& d)
  {
    value += string_cast(d);
  }

  virtual
  void
  operator()(const libport::opts::FileData& d)
  {
    value += string_cast(d);
  }

  std::string value;
};

void
check_exp_and_file()
{
  OptionParser p;
  p << libport::opts::file
    << libport::opts::exp;

  // Test presence
  {
    args_type args;
    args << "-e" << "111"
      //         << "-e222"
         << "-f" << "333"
      //         << "-f444"
         << "--expression" << "555"
         << "--expression=666"
         << "--file" << "777"
         << "--file=888"
      ;

    p(args);
    DataCollector collect;
    collect(libport::opts::input_arguments);
    BOOST_CHECK_EQUAL(collect.value,
                      "expression {{{111}}}"
                      // "expression {{{222}}}"
                      "file `333'"
                      // "file `444'"
                      "expression {{{555}}}"
                      "expression {{{666}}}"
                      "file `777'"
                      "file `888'");
  }
}

test_suite*
init_test_suite()
{
  libport::program_initialize("test_suite");
  test_suite* suite = BOOST_TEST_SUITE("libport::input-arguments");
  suite->add(BOOST_TEST_CASE(check_exp_and_file));
  return suite;
}
