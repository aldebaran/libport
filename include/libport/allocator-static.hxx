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
  template <typename Exact, size_t Chunk>
  void*
  StaticallyAllocated<Exact, Chunk>::operator new(size_t size)
  {
# if DEBUG_SA_UNIQ_THREAD_CHECK
    aver_eq(thread, pthread_self());
# endif
    LIBPORT_USE(size);

    // Allocations exceeds the chunk size.
    assert_le(size, Exact::allocator_static_max_size);

    if (size_ == storage_size_)
      _grow();

    size_t where = where_; // Avoid volatile side-effects.

    // Number of allocated area exceeds the storage capacity.
    assert_le(size_, storage_size_);

    size_++;
    void* res = 0;
    std::swap(res, pointers_[where]);
    POOL_ALLOC(pool_header, res, size);
    aver(res);

    // Do not use a modulo here because this is costly.
    assert_le(where, storage_size_);
    if (++where == storage_size_)
      where = 0;

    // Store after usage.
    where_ = where;
    return res;
  }

  template <typename Exact, size_t Chunk>
  void
  StaticallyAllocated<Exact, Chunk>::operator delete(void* obj)
  {
# if DEBUG_SA_UNIQ_THREAD_CHECK
    aver_eq(thread, pthread_self());
# endif
    aver(obj != 0);

    // Avoid volatile side-effects.
    size_t size = size_;

    // Avoid underflow caused by (where_ - size_)
    size_t index = where_;
    if (index < size)
      index += storage_size_;
    index -= size;

    std::swap(pointers_[index], obj);
    POOL_FREE(pool_header, pointers_[index]);
    aver(obj == 0);
    size--;

    // Store after usage.
    size_ = size;
  }

  template <typename Exact, size_t Chunk>
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
    for (size_t i = 0; i < chunk_size_; ++i)
      pointers_[storage_size_ + i] =
        pool + i * Exact::allocator_static_max_size;
    where_ = storage_size_;
    storage_size_ += chunk_size_;
    chunk_size_ *= 2;
  }

  template <typename Exact, size_t Chunk>
  std::vector<void*> StaticallyAllocated<Exact, Chunk>::pointers_;

  template <typename Exact, size_t Chunk>
  volatile size_t StaticallyAllocated<Exact, Chunk>::where_ = 0;

  template <typename Exact, size_t Chunk>
  volatile size_t StaticallyAllocated<Exact, Chunk>::size_ = 0;

  template <typename Exact, size_t Chunk>
  volatile size_t StaticallyAllocated<Exact, Chunk>::storage_size_ = 0;

  template <typename Exact, size_t Chunk>
  size_t StaticallyAllocated<Exact, Chunk>::chunk_size_ = Chunk;

# if DEBUG_SA_UNIQ_THREAD_CHECK
  template <typename Exact, size_t Chunk>
  pthread_t StaticallyAllocated<Exact, Chunk>::thread = pthread_self();
# endif

# ifndef NVALGRIND
  template <typename Exact, size_t Chunk>
  int* StaticallyAllocated<Exact, Chunk>::initialize_pool()
  {
    int* header = reinterpret_cast<int*>(malloc(sizeof(int)));
    POOL_CREATE(header, 0, false);
    return header;
  }

  template <typename Exact, size_t Chunk>
  int* StaticallyAllocated<Exact, Chunk>::pool_header = initialize_pool();
# endif
}

#endif
