#include <iostream>
#include <libport/singleton-ptr.hh>

using namespace libport;

struct foo
{
  void bar()
  {
    std::cout << "foo::bar()" << std::endl;
  }
};

STATIC_INSTANCE_DECL(foo, test1);
STATIC_INSTANCE_DECL(foo, test2);

int
main()
{
  test1->bar();
  (*test1).bar();

  test2->bar();
  (*test2).bar();

  return 0;
}
