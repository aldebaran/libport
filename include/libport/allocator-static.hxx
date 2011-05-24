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

# include <libport/algorithm>
# include <libport/cassert>

namespace libport
{
  template <typename Exact,unsigned Chunk>
  void*
  StaticallyAllocated<Exact, Chunk>::operator new(size_t size)
  {
# if DEBUG_SA_UNIQ_THREAD_CHECK
    aver_eq(thread, pthread_self());
# endif
    LIBPORT_USE(size);

    // Allocations exceeds the chunk size.
    assert_le(size / sizeof(long long), Exact::allocator_static_max_size);

    if (size_ == storage_size_)
      _grow();
    // Allocations exceeds the chunk size.
    assert_le(size_, storage_size_);

    size_++;
    void* res = 0;
    std::swap(res, pointers_[where_]);
    POOL_ALLOC(pool_header, res, size);
    aver(res);
    where_ = (where_ + 1) % storage_size_;
    return res;
  }

  template <typename Exact, unsigned Chunk>
  void
  StaticallyAllocated<Exact, Chunk>::operator delete(void* obj)
  {
# if DEBUG_SA_UNIQ_THREAD_CHECK
    aver_eq(thread, pthread_self());
# endif
    aver(obj != 0);
    unsigned w = where_ - size_;

    // Fix overflow // i-e w < 0, or where_ < size_
    if (w > storage_size_)
      w += storage_size_;
    std::swap(pointers_[w], obj);
    POOL_FREE(pool_header, pointers_[w]);
    aver(obj == 0);
    size_--;
  }

  template <typename Exact, unsigned Chunk>
  void
  StaticallyAllocated<Exact, Chunk>::_grow()
  {
# if DEBUG_SA_UNIQ_THREAD_CHECK
    aver_eq(thread, pthread_self());
# endif
    char* pool = reinterpret_cast<char*>
      (malloc(chunk_size_ * Exact::allocator_static_max_size));
    if (!pool)
      throw std::bad_alloc();
    pointers_.resize(storage_size_ + chunk_size_);
    for (unsigned i = 0; i < chunk_size_; ++i)
    {
      pointers_[storage_size_ + i] =
        pool + i * Exact::allocator_static_max_size;
      *(unsigned*) pointers_[storage_size_ + i] = 0x0C0C0C0C;
    }
    where_ = storage_size_;
    storage_size_ += chunk_size_;
    chunk_size_ *= 2;
  }

  template <typename Exact, unsigned Chunk>
  std::vector<void*> StaticallyAllocated<Exact, Chunk>::pointers_;

  template <typename Exact, unsigned Chunk>
  volatile unsigned StaticallyAllocated<Exact, Chunk>::where_ = 0;

  template <typename Exact, unsigned Chunk>
  volatile unsigned StaticallyAllocated<Exact, Chunk>::size_ = 0;

  template <typename Exact, unsigned Chunk>
  volatile unsigned StaticallyAllocated<Exact, Chunk>::storage_size_ = 0;

  template <typename Exact, unsigned Chunk>
  unsigned StaticallyAllocated<Exact, Chunk>::chunk_size_ = Chunk;

# if DEBUG_SA_UNIQ_THREAD_CHECK
  template <typename Exact, unsigned Chunk>
  pthread_t StaticallyAllocated<Exact, Chunk>::thread = pthread_self();
# endif

# ifndef NVALGRIND
  template <typename Exact, unsigned Chunk>
  int* StaticallyAllocated<Exact, Chunk>::initialize_pool()
  {
    int* header = reinterpret_cast<int*>(malloc(sizeof(int)));
    POOL_CREATE(header, 0, false);
    return header;
  }

  template <typename Exact, unsigned Chunk>
  int* StaticallyAllocated<Exact, Chunk>::pool_header = initialize_pool();
# endif
}

#endif
