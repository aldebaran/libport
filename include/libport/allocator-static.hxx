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
  template <typename Exact,unsigned Chunk>
  void*
  StaticallyAllocated<Exact, Chunk>::operator new(size_t size)
  {
    (void)size;

    // Allocations exceeds the chunk size.
    assert_le(size / sizeof(long long), Size);

    if (size_ == storage_size_)
      _grow();
    // Allocations exceeds the chunk size.
    assert_le(size_, storage_size_);

    size_++;
    void* res = pointers_[where_];
    where_ = (where_ + 1) % storage_size_;
    return res;
  }

  template <typename Exact, unsigned Chunk>
  void
  StaticallyAllocated<Exact, Chunk>::operator delete(void* obj)
  {
    unsigned w = where_ - size_;
    // Fix overflow
    if (w > storage_size_)
      w += storage_size_;
    pointers_[w] = obj;
    size_--;
  }

  template <typename Exact, unsigned Chunk>
  void
  StaticallyAllocated<Exact, Chunk>::_grow()
  {
    char* pool = reinterpret_cast<char*>
      (malloc(Chunk * Exact::allocator_static_max_size));
    if (!pool)
      throw std::bad_alloc();
    pointers_.resize(storage_size_ + Chunk);
    for (unsigned i = 0; i < Chunk; ++i)
      pointers_[storage_size_ + i] =
        pool + i * Exact::allocator_static_max_size;
    where_ = storage_size_;
    storage_size_ += Chunk;
  }

  template <typename Exact, unsigned Chunk>
  std::vector<void*> StaticallyAllocated<Exact, Chunk>::pointers_;

  template <typename Exact, unsigned Chunk>
  unsigned StaticallyAllocated<Exact, Chunk>::where_ = 0;

  template <typename Exact, unsigned Chunk>
  unsigned StaticallyAllocated<Exact, Chunk>::size_ = 0;

  template <typename Exact, unsigned Chunk>
  unsigned StaticallyAllocated<Exact, Chunk>::storage_size_ = 0;
}

#endif
