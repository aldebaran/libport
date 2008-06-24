#include <vector>
#include <sstream>
#include <iostream>
#include <boost/assign.hpp>
using namespace boost::assign;

#include <libport/foreach.hh>

#define ECHO(S)                                 \
  std::cerr << S << std::endl

#define CHECK(Container, Foreach, Expect)       \
  do {                                          \
    Container<int> v = list_of(1)(2)(3)(4);     \
    std::stringstream o;                        \
    Foreach (int i, v)                          \
      o << i << ',';                            \
    ECHO("Expected  :" << Expect);              \
    ECHO("Effective :" << o.str());             \
    assert(o.str() == Expect);                  \
  } while (0)

int
main()
{
  CHECK(      std::vector,  foreach, "1,2,3,4,");
  CHECK(const std::vector,  foreach, "1,2,3,4,");
  CHECK(      std::vector, rforeach, "4,3,2,1,");
  CHECK(const std::vector, rforeach, "4,3,2,1,");
}
