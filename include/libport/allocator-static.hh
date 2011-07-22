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

// This macro guards assertions which are verifying that the statically
// allocated is still used in one uniq thread.
//
// Windows compilation of urbi makes this assertion fail when urbi.exe is
// killed with 2 Ctrl-C, which cause another thread to make 3 deletions.
// Due to the lack of debugging symbols and to the un-risky circumstances in
// which the bug appear, it seems to be easier to ignore this assertion and
// rely Linux builds to detect logical errors.
//
// see Bug #4672
//
# if !defined NDEBUG && !defined WIN32
#  define DEBUG_SA_UNIQ_THREAD_CHECK 1
# else
#  define DEBUG_SA_UNIQ_THREAD_CHECK 0
# endif


# if DEBUG_SA_UNIQ_THREAD_CHECK
#  include <libport/pthread.h>
# endif
# include <libport/instrument.hh>

namespace libport
{
  template <typename Exact, size_t Chunk>
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
    static volatile size_t where_;
    // Number of objects currently allocated.
    static volatile size_t size_;
    // Number of objects slots available.
    static volatile size_t storage_size_;
    // Current chunk size to allocate.
    static size_t chunk_size_;

# if DEBUG_SA_UNIQ_THREAD_CHECK
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
