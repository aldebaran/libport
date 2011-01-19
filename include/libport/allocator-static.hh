/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef ALLOCATOR_STATIC_HH
# define ALLOCATOR_STATIC_HH

namespace libport
{
  template <unsigned Max, unsigned Size>
  class StaticallyAllocated
  {
  public:
    void* operator new(unsigned int);
    void operator delete(void* obj);

    static int initialize();

    // The objects memory pool.
    static char objects_[Size * Max];
    // The ring of free memory slots indexes.
    static unsigned pointers_[Max];
    // Pointer
    static unsigned where_;
    // Number of objects currently allocated.
    static unsigned size_;
  };
}

# include <libport/allocator-static.hxx>

#endif
