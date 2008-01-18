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
  operator<<(std::ostream&, const exit& e)
  {
    e();
  }

}
