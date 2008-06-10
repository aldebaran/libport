#include <vector>
#include <libport/containers.hh>

using namespace libport;

static std::vector<unsigned> v;

static bool
odd(unsigned i)
{
  return i % 2 == 0;
}

int main()
{
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  assert(v.size() == 5);
  erase_if(v, odd);
  assert(v.size() == 3);
  assert(libport::has(v, 1));
  assert(libport::has(v, 3));
  assert(libport::has(v, 5));
}
