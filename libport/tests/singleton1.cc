#include <iostream>
#include "libport/singleton-ptr.hh"

using namespace urbi;

struct foo {
  void bar()
  {
    std::cout << "foo::bar()" << std::endl;
  }
};

int main()
{
  SingletonPtr<foo> test = SingletonPtr<foo>();
  test->bar();
  (*test).bar();
  return 0;
}
