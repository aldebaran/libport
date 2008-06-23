/**
 ** Test code for libport/shared-ptr.hh features.
 */

#include <iostream>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>

#include <libport/ref-counted.hh>
#include <libport/shared-ptr.hh>

using libport::shared_ptr;

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
typedef shared_ptr<Counted, true> rCounted;
BOOST_CLASS_TRACKING(rCounted, boost::serialization::track_never)

int
main()
{
  rCounted a = new Counted(42);
  std::stringstream s;

  assert(ninstances == 1);

  boost::archive::text_oarchive oa(s);
  oa << a << a;

  assert(ninstances == 1);

  boost::archive::text_iarchive ia(s);
  rCounted b1, b2;
  ia >> b1 >> b2;

  assert(ninstances == 2);
  assert(b1.get() != a.get());
  a = 0;

  assert(ninstances == 1);

  assert(b1->val == 42);
  assert(b2->val == 42);
  assert(b1.get() == b2.get());

  b1 = 0;
  assert(ninstances == 1);

  b2 = 0;
  assert(ninstances == 0);

  return 0;
}
