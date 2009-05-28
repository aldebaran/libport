#ifndef LIBPORT_SMART_ALLOCATED_HH
# define LIBPORT_SMART_ALLOCATED_HH

namespace libport
{
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
    static unsigned current_;
    static unsigned free_;
    static T* map_ [Max];
  };
}

#endif
