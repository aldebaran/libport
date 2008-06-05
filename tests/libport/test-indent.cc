/**
 ** Test code for libport/indent.hh features.
 */

#include <iostream>
#include <sstream>
#include <libport/contract.hh>
#include <libport/indent.hh>

using libport::iendl;
using libport::incendl;
using libport::decendl;

int main ()
{
  std::ostringstream s;

  s << "{" << incendl
    << "1," << iendl
    << "2," << iendl
    << "{" << incendl
    << "2.1," << iendl
    << "2.2" << decendl
    << "}," << iendl
    << "3" << decendl
    << "}" << std::endl;

  std::string expected = "{\n\
  1,\n\
  2,\n\
  {\n\
    2.1,\n\
    2.2\n\
  },\n\
  3\n\
}\n\
";
  assertion (s.str () == expected);

  return 0;
}
