/*
 * Copyright (C) 2008-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

/**
 ** Test the timing nested tasks.
 */

#include <iostream>
#include <libport/unistd.h>
#include <libport/timer.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

static
void
msleep(int milliseconds)
{
  usleep(milliseconds * 1000);
}

void
check ()
{
  enum timevar
  {
    One = 1,
    Two,
    Three
  };

  libport::timer t;
  t.name (One, "One");
  t.name (Two, "Two");
  t.name (Three, "Three");

  t.start ();

  t.push (One);
  msleep (100);
  t.pop (1);

  t.push ("Two");
  msleep (200);
  t.pop ("Two");

  t.push ("Three");
  msleep (300);
  t.pop (Three);

  t.stop ();
  t.dump (std::cerr);

  // Also check that Valgrind is happy with dump on destruction.
  t.dump_on_destruction(std::cout);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::timer");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
