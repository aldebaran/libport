#include <libport/singleton-ptr.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

struct foo
{
  foo()
    : val(0)
  {}

  int bar()
  {
    return val++;
  }
  int val;
};

STATIC_INSTANCE_DECL(foo, f1);
STATIC_INSTANCE_DECL(foo, f2);

void
check()
{
  BOOST_CHECK_EQUAL(f1->bar(), 0);
  BOOST_CHECK_EQUAL(f1->bar(), 1);
  BOOST_CHECK_EQUAL((*f1).bar(), 2);

  BOOST_CHECK_EQUAL(f2->bar(), 0);
  BOOST_CHECK_EQUAL(f2->bar(), 1);
  BOOST_CHECK_EQUAL((*f2).bar(), 2);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::singleton-ptr test suite");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
