#include <cassert>
#include <vector>
#include <boost/lambda/lambda.hpp>
#include <libport/containers.hh>

static std::vector<int> v;

static void
check(int i, bool b)
{
  (void) i;
  (void) b;
  assert(b == libport::has_if(v, boost::lambda::_1 == i));
}

int main()
{
  v.push_back(1);
  v.push_back(2);
  check(1, true);
  check(2, true);
  check(3, false);
  return 0;
}
