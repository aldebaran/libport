#include <iostream>
#include <libport/ufloat.h>

# define TESTME(x)					\
	std::cout << #x << ": "				\
	<< ((x) ? "true" : "false") << std::endl


int main()
{
  ufloat f;
  ufloat g;

  f = 1.0;
  g = 10.0;

  TESTME(f < g);
  TESTME(g > f);
  TESTME(f == f);
  TESTME(f != g);

  f = g = 42.0;

  TESTME(f == g);
  TESTME(f <= g);
  TESTME(f >= g);

  TESTME(libport::ufloat_cast<int>(f) == int(1));
  TESTME(libport::ufloat_cast<int>(g) == int(10));

  TESTME(libport::ufloat_cast<int>(-f) == int(-1));
  TESTME(libport::ufloat_cast<int>(-g) == int(-10));

  TESTME(libport::ufloat_cast<size_t>(f) == size_t(1));
  TESTME(libport::ufloat_cast<size_t>(g) == size_t(10));

  return 0;
}
