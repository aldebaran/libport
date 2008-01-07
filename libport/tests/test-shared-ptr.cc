/**
 ** Test code for libport/shared-ptr.hh features.
 */

// FIXME: ungoing, unfinished work.

#include <iostream>
#include <libport/shared-ptr.hh>

#define ECHO(S) std::cerr << __LINE__ << ": " #S " = " << S << std::endl
#define INSTANCES() ECHO(Counted::instances)

using libport::shared_ptr;

struct Counted
{
  Counted () { ++instances; INSTANCES(); }
  virtual ~Counted () { assert (0 < instances); --instances; INSTANCES();}
  static unsigned instances;
};

struct SubCounted : Counted
{
};

unsigned Counted::instances;

typedef shared_ptr<Counted> rCounted;
typedef shared_ptr<SubCounted> rSubCounted;

int
main ()
{
  {
    SubCounted* subcounted = new SubCounted;
    rSubCounted r1 (subcounted);
    {
      INSTANCES();
      rCounted r2;
      INSTANCES();
      r2 = r1;
      INSTANCES();
      rSubCounted r3 = r2.cast<SubCounted>();
      INSTANCES();
    }
    INSTANCES();
  }
  INSTANCES();
}
