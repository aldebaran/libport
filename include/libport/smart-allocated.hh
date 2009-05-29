#ifndef LIBPORT_SMART_ALLOCATED_HH
# define LIBPORT_SMART_ALLOCATED_HH
#include <cstdlib>
#include <stdexcept>
namespace libport
{
  template <size_t Max>
  class MultiSmartAllocated
  {
  public:
    static void* operator new(size_t size);
    static void operator delete(void* ptr);
  };

  template <typename T, size_t Max>
  class SmartAllocated
  {
  public:
    static void* operator new(size_t size);
    static void operator delete(void* ptr);

  private:
    class Initializer
    {
    public:
      Initializer();
    };
    //position of next *free* slot in map
    static unsigned current_;
    //sizo of map
    static unsigned size_;
    static T** map_;
  };
}
#include <libport/smart-allocated.hxx>
#endif
