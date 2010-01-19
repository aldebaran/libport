/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */
/**
 ** Test code for libport/intrusive-ptr.hh features.
 */

#include <libport/ref-counted.hh>
#include <libport/intrusive-ptr.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

using libport::intrusive_ptr;

struct Counted : libport::RefCounted
{
  Counted () : libport::RefCounted() { ++instances; }
  virtual ~Counted () { assert (0 < instances); --instances; }
  static unsigned instances;
};

struct SubCounted : Counted
{
};

unsigned Counted::instances;

typedef intrusive_ptr<Counted> rCounted;
typedef intrusive_ptr<SubCounted> rSubCounted;

void
check()
{
# define CHECK(N)						\
  BOOST_CHECK_EQUAL(Counted::instances, unsigned(N))

  {
    SubCounted* subcounted = new SubCounted;
    rSubCounted r1 (subcounted);
    {
      CHECK(1);
      rCounted r2;
      CHECK(1);
      r2 = r1;
      CHECK(1);
      rSubCounted r3 = r2.cast<SubCounted>();
      CHECK(1);
      rCounted r4 = new Counted;
      CHECK(2);
      r4 = r3;
      CHECK(1);
    }
    CHECK(1);
  }
  CHECK(0);

  {
    rSubCounted p = new SubCounted();
    p = p;
    CHECK(1);
  }
  CHECK(0);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::cli");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
