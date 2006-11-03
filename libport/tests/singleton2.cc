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
  SingletonPtr<foo> test1 = SingletonPtr<foo>();
  SingletonPtr<foo> test2 = SingletonPtr<foo>();

  test1->bar();
  (*test1).bar();

  test2->bar();
  (*test2).bar();

  if (&test1 == &test2)
    return 1;
  return 0;
}
