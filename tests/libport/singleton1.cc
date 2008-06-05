#include <iostream>
#include <libport/singleton-ptr.hh>

struct foo
{
  void bar()
  {
    std::cout << "foo::bar()" << std::endl;
  }
};

STATIC_INSTANCE_DECL(foo, test);

int main()
{
  test->bar();
  (*test).bar();
  return 0;
}
