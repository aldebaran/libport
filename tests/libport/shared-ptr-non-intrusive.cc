/**
 ** Test code for libport/shared-ptr.hh features.
 */

#include <libport/shared-ptr.hh>

#define INSTANCES(N) assert(Counted::instances == N)

using libport::shared_ptr;

struct Counted : libport::RefCounted
{
  Counted () : libport::RefCounted() { ++instances; }
  virtual ~Counted () { assert (0 < instances); --instances; }
  static unsigned instances;
};

unsigned Counted::instances;

typedef shared_ptr<Counted, false> rCounted;

int
main ()
{
  {
    Counted* counted = new Counted;
    rCounted r1 (counted);
    {
      INSTANCES(1);
      rCounted r2;
      INSTANCES(1);
      r2 = r1;
      INSTANCES(1);
      rCounted r3 = new Counted;
      INSTANCES(2);
      r3 = r2;
      INSTANCES(1);
    }
    INSTANCES(1);
  }
  INSTANCES(0);
}
