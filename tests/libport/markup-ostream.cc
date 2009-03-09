#include <iostream>

#include <libport/markup-ostream.hh>
#include <libport/unit-test.hh>

using libport::test_suite;
using namespace libport;

static void test()
{
  std::stringstream stream;
  MarkupOStream os(stream);

  os << table << "Can"         << col << "I"   << col << "haz"
     << row   << "cheezebuger" << col << "plz" << col << "mister"
     << row   << "hoomin"      << col << "?"
     << etable;

  BOOST_CHECK_EQUAL(stream.str(),
                    "Can         I   haz\n"
                    "cheezebuger plz mister\n"
                    "hoomin      ?\n");
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::base64 test suite");
  suite->add(BOOST_TEST_CASE(test));
  return suite;
}
