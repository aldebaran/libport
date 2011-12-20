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
 ** Test libport::backtrace.
 */

#include <libport/backtrace.hh>
#include <libport/foreach.hh>
#include "test.hh"
#include <libport/compiler.hh>

#include <iostream>

using libport::test_suite;
using namespace libport;

namespace freefunction
{
  ATTRIBUTE_NOINLINE
  void
  inner()
  {
    foreach(const std::string& s, backtrace())
      std::cout << s << std::endl;
  }

  ATTRIBUTE_NOINLINE
  void
  outer()
  {
    inner();
  }

  void
  check()
  {
    outer();
  }
}

namespace object
{
  ATTRIBUTE_NOINLINE
  void
  inner()
  {
    std::cout << Backtrace() << std::endl;
  }

  ATTRIBUTE_NOINLINE
  void
  outer()
  {
    inner();
  }

  void
  check()
  {
    outer();
  }
}

test_suite*
init_test_suite()
{
  skip_if("Wine");
  test_suite* suite = BOOST_TEST_SUITE("libport::backtrace");
  suite->add(BOOST_TEST_CASE(freefunction::check));
  suite->add(BOOST_TEST_CASE(object::check));
  return suite;
}
