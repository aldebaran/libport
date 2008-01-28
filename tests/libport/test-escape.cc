/**
 ** Test code for libport/escape.hh.
 */

#include "libport/escape.hh"
#include "libport/assert.hh"
#include <sstream>
#include <iostream>

using libport::escape;

int main ()
{
  std::ostringstream s;

  s << escape ("\a\b\f\n\r\t\v\\\"") << escape ('\a');

  passert (s.str(), s.str () == "\\x07\\b\\f\\n\\r\\t\\v\\\\\\\"\\x07");
}
