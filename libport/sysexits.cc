#include <cstdlib>
#include "libport/sysexits.hh"

namespace libport
{
  exit::exit(unsigned s)
    : status_(s)
  {}

  void 
  exit::operator() () const
  {
    std::exit (status_);
  }

  std::ostream&
  operator<<(std::ostream& o, const exit& e)
  {
    e();
    //vc++ does not detect that it is not required as we have exited by now
    return o;
  }

}

