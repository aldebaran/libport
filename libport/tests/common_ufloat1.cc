#include <iostream>
#include "libport/ufloat.h"

// Make sure we are using the test config.h, that does not define
// the UFLOAT kind.
#ifndef LIBPORT_TEST
# error we are using the libport/config.h to be installed instead of \
	the one meant to be used for tests
#endif

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
