/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef ALLOCATOR_STATIC_HXX
# define ALLOCATOR_STATIC_HXX

# include <iostream>

# include <libport/cassert>

namespace libport
{
  template <unsigned Max, unsigned Size>
  void*
  StaticallyAllocated<Max, Size>::operator new(unsigned int size)
  {
    static int initialized = initialize();
    (void)initialized;
    (void)size;

    assert_le(size, Size); // Allocations exceeds the chunk size.
    if (size_ == Max) // Too many objects allocated.
      throw std::bad_alloc();

    size_++;
    void* res = &objects_[pointers_[where_] * Size];
    where_ = (where_ + 1) % Max;
    return res;
  }

  template <unsigned Max, unsigned Size>
  void
  StaticallyAllocated<Max, Size>::operator delete(void* obj)
  {
    assert_ne(size_, 0u);
    unsigned n = ((char*)obj - (char*)&objects_) / Size;
    unsigned w = where_ - size_;
    // Fix overflow
    if (w > Max)
      w += Max;
    pointers_[w] = n;
    size_--;
  }

  template <unsigned Max, unsigned Size>
  int
  StaticallyAllocated<Max, Size>::initialize()
  {
    for (unsigned i = 0; i < Max; ++i)
      pointers_[i] = i;
    where_ = size_ = 0;
    return 0;
  }

  template <unsigned Max, unsigned Size>
  char StaticallyAllocated<Max, Size>::objects_[Size * Max];
  template <unsigned Max, unsigned Size>
  unsigned StaticallyAllocated<Max, Size>::pointers_[Max];
  template <unsigned Max, unsigned Size>
  unsigned StaticallyAllocated<Max, Size>::where_;
  template <unsigned Max, unsigned Size>
  unsigned StaticallyAllocated<Max, Size>::size_;
}

#endif
