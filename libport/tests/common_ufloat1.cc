#include <iostream>
#include "libport/ufloat.h"

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

  return 0;
}
