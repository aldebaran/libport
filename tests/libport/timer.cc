/**
 ** Test the timing nested tasks.
 */

#include <iostream>
#include <libport/unistd.h>
#include <libport/timer.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

void
check ()
{
  libport::timer t;
  enum timevar
    {
    One = 1,
    Two,
    Three
  };

  t.name (One, "One");
  t.name (Two, "Two");
  t.name (Three, "Three");

  t.start ();

  t.push (One);
  sleep (1);
  t.pop (1);

  t.push ("Two");
  sleep (2);
  t.pop ("Two");

  t.push ("Three");
  sleep (3);
  t.pop (Three);

  t.stop ();
  t.dump (std::cerr);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::timer");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
