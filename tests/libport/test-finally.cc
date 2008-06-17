#include <cassert>
#include <libport/finally.hh>

int main()
{
  int var = 1;
  {
    libport::Finally finally(libport::restore(var));
    assert(var == 1);
    var = 2;
    assert(var == 2);
  }
  assert(var == 1);
  return 0;
}
