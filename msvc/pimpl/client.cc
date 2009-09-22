#include <iostream>
#include "auto_pimpl.h"

int main ()
{
  pimpl  p;

  std::cout << p.get_value() << std::endl;

  return 0;
}
