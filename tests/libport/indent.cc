/*
 * Copyright (C) 2007-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** Test code for libport/indent.hh features.
 */

#include <iostream>
#include <sstream>
#include <libport/indent.hh>

#include <libport/unit-test.hh>

using libport::iendl;
using libport::incendl;
using libport::decendl;

using libport::test_suite;

void
check ()
{
  std::ostringstream s;

  s << "{" << incendl
    << "1," << iendl
    << "2," << iendl
    << "{" << incendl
    << "2.1," << iendl
    << "2.2" << decendl
    << "}," << iendl
    << "3" << decendl
    << "}" << std::endl;

  std::string expected = "{\n\
  1,\n\
  2,\n\
  {\n\
    2.1,\n\
    2.2\n\
  },\n\
  3\n\
}\n\
";

  BOOST_CHECK_EQUAL(s.str (), expected);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::indent");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
