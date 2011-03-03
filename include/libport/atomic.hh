/*
 * Copyright (C) 2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#if defined(_MSC_VER)
# include <Windows.h>
# include <Winbase.h>
#endif

namespace libport
{
  namespace atomic
  {
#if defined(__GNUC__) && __GNUC__ < 4 && __arm__
# if 0 // Those are provided by the uclibc, but don't work :-(
# include <atomic.h>
    inline long fetch_increment(long *ptr)
    {
      return atomic_exchange_and_add(ptr, 1);
    }

    inline long fetch_decrement(long *ptr)
    {
      return atomic_exchange_and_add(ptr, -1
    }

    inline long increment_fetch(long *ptr)
    {
      return fetch_increment(ptr) + 1;
    }

    inline long decrement_fetch(long *ptr)
    {
      return fetch_decrement(ptr) - 1;
    }
# endif
#elif defined(__GNUC__)
    inline long increment_fetch(long* ptr)
    {
      return __sync_add_and_fetch(ptr, 1);
    }

    inline long decrement_fetch(long* ptr)
    {
      return __sync_sub_and_fetch(ptr, 1);
    }

    inline long fetch_increment(long* ptr)
    {
      return __sync_fetch_and_add(ptr, 1);
    }

    inline long fetch_decrement(long* ptr)
    {
      return __sync_fetch_and_sub(ptr, 1);
    }
#elif defined(_MSC_VER)
    inline long increment_fetch(long* ptr)
    {
      return InterlockedIncrement(ptr);
    };

    inline long decrement_fetch(long* ptr)
    {
      return InterlockedDecrement(ptr);
    };

    inline long fetch_increment(long* ptr)
    {
      return increment_fetch(ptr) - 1;
    }

    inline long fetch_decrement(long* ptr)
    {
      return decrement_fetch(ptr) + 1;
    }
#endif
  }
}
