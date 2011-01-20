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

# include <cstdlib>

namespace libport
{
  template <unsigned Max, size_t Size>
  class StaticallyAllocated
  {
  public:
    void* operator new(size_t);
    void operator delete(void* obj);

    static int initialize();

    enum
    {
      chunk_size = (Size - 1) / sizeof(long long) + 1,
    };

    // The objects memory pool.
    static long long objects_[chunk_size * Max];
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
