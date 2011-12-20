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

#include <iostream>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>

#include <libport/ref-counted.hh>
#include <libport/intrusive-ptr.hh>

#include <libport/unit-test.hh>

using libport::test_suite;

static int ninstances;

struct Counted : libport::RefCounted
{
  Counted () { val = 0; ++ninstances; };
  Counted (int n) { val = n; ++ninstances; };
  virtual ~Counted() { --ninstances; };
  int val;
  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive& ar, const unsigned int /* version */) {
    ar & val;
  }
};
typedef libport::intrusive_ptr<Counted> rCounted;
BOOST_CLASS_TRACKING(rCounted, boost::serialization::track_never)

void
check()
{
# define CHECK(N)						\
  BOOST_CHECK_EQUAL(ninstances, N)

  rCounted a = new Counted(42);
  std::stringstream s;

  CHECK(1);

  boost::archive::text_oarchive oa(s);
  oa << a << a;

  CHECK(1);

  boost::archive::text_iarchive ia(s);
  rCounted b1, b2;
  ia >> b1 >> b2;

  CHECK(2);
  BOOST_CHECK_NE(b1.get(), a.get());
  a = 0;

  CHECK(1);

  BOOST_CHECK_EQUAL(b1->val, 42);
  BOOST_CHECK_EQUAL(b2->val, 42);
  BOOST_CHECK_EQUAL(b1.get(), b2.get());

  b1 = 0;
  CHECK(1);

  b2 = 0;
  CHECK(0);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport::intrusive-ptr-serialize");
  suite->add(BOOST_TEST_CASE(check));
  return suite;
}
