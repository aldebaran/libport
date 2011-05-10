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

# include <vector>
# include <cstdlib>

# if !defined NDEBUG
#  include <libport/pthread.h>
# endif
# include <libport/instrument.hh>

namespace libport
{
  template <typename Exact, unsigned Chunk>
  class StaticallyAllocated
  {
  public:
    void* operator new(size_t);
    void operator delete(void* obj);

  private:
    // Increase the storage size by Chunk objects.
    static void _grow();
    // The ring of free memory slots.
    static std::vector<void*> pointers_;
    // Pointer
    static unsigned where_;
    // Number of objects currently allocated.
    static unsigned size_;
    // Number of objects slots available.
    static unsigned storage_size_;
    // Current chunk size to allocate.
    static unsigned chunk_size_;

# if !defined NDEBUG
  public:
    // The only thread allow to access the static allocator.  This field is
    // public to be redefined when another thread is manipulating the
    // allocator safely.
    static pthread_t thread;
# endif

# ifndef NVALGRIND
    // This is necessary to identify the pool in Valgrind.
    static int* initialize_pool();
    static int* pool_header;
# endif
  };
}

# include <libport/allocator-static.hxx>

#endif
