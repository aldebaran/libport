/**
 ** Test code for libport::Deref.
 */

#include <sstream>
#include <libport/deref.hh>

using libport::deref;

int main ()
{
  std::ostringstream s1;
  std::ostringstream s2;

  int i = 123;
  const int& ic = i;

  s1 << deref << i << deref << &i << deref << ic << deref << &ic;
  s2 << i << i << ic << ic;

  postcondition (s1.str () == s2.str ());

  return 0;
}
