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
  virtual ~Counted () { aver(0 < instances); --instances; }
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

class RefFromDtor: public libport::RefCounted
{
public:
  RefFromDtor()
    : dead_(false)
  {}

  ~RefFromDtor()
  {
    BOOST_CHECK(!dead_);
    dead_ = true;
    libport::intrusive_ptr<RefFromDtor> ref_from_dtor = this;
  }

private:
  bool dead_;
};

void
ref_from_dtor()
{
  libport::intrusive_ptr<RefFromDtor> ref_from_dtor = new RefFromDtor;
}


class HeldRefFromDtor: public libport::RefCounted
{
public:
  HeldRefFromDtor()
  {}

  static libport::intrusive_ptr<HeldRefFromDtor> held;
  ~HeldRefFromDtor()
  {
    held = this;
  }
};
libport::intrusive_ptr<HeldRefFromDtor> HeldRefFromDtor::held;

void
held_ref_from_dtor()
{
  libport::intrusive_ptr<HeldRefFromDtor> held_ref_from_dtor = new HeldRefFromDtor;
  BOOST_CHECK_THROW(held_ref_from_dtor = 0, std::exception);
}

class Warded: public libport::RefCounted
{
public:
  typedef libport::intrusive_ptr<Warded> rWarded;
  static void evil(rWarded warded)
  {
    warded->x++;
  }

  Warded()
    : x(0)
  {
    Ward ward(this);
    evil(this);
  }

  ~Warded()
  {
    x = 0;
  }

  int x;
};

void ward()
{
  Warded w;
  BOOST_CHECK_EQUAL(w.x, 1);
}

test_suite*
init_test_suite()
{
  libport::abort_throw();

  test_suite* suite = BOOST_TEST_SUITE("libport::intrusive_ptr");
  suite->add(BOOST_TEST_CASE(check));
  suite->add(BOOST_TEST_CASE(ref_from_dtor));
#ifndef NDEBUG
  suite->add(BOOST_TEST_CASE(held_ref_from_dtor));
#endif
  suite->add(BOOST_TEST_CASE(ward));
  return suite;
}
