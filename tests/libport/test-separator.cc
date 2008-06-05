/**
 ** Test code for libport/separator.hh features.
 */

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <libport/contract.hh>
#include <libport/separator.hh>
#include <libport/pair.hh>

int main ()
{
  {
    std::vector<int> v (4);
    std::fill (v.begin (), v.end (), 51);

    std::ostringstream s;
    s << libport::separate (v, ", ");
    assertion (s.str () == "51, 51, 51, 51");
  }

  {
    std::vector<int> v (4);
    std::fill (v.begin (), v.end (), 51);

    std::ostringstream s;
    s << libport::separate (v, libport::make_pair (",", " "));
    assertion (s.str () == "51, 51, 51, 51");
  }

  {
    int p = 51;
    std::vector<int*> v (4);
    std::fill (v.begin (), v.end (), &p);

    std::ostringstream s;
    s << libport::separate (v, ", ");
    assertion (s.str () == "51, 51, 51, 51");
  }

  return 0;
}
