/*
 * Copyright (C) 2009, 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_SMART_ALLOCATED_HH
# define LIBPORT_SMART_ALLOCATED_HH
#include <libport/cstdlib>
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
