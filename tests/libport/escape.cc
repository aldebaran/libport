/**
 ** Test code for libport/escape.hh.
 */

#include <sstream>
#include <libport/assert.hh>
#include <libport/escape.hh>

using libport::escape;

int main ()
{
  std::ostringstream s;

  s << escape("\a\b\f\n\r\t\v\\\"");
  s << escape('\a');

  passert(s.str(), s.str() == "\\x07\\b\\f\\n\\r\\t\\v\\\\\\\"\\x07");
}
