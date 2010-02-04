/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef LIBPORT_THREAD_DATA_HH
# define LIBPORT_THREAD_DATA_HH

# include <libport/local-data.hh>
# include <libport/specific-ptr.hh>
# include <libport/pthread.h>

namespace libport
{
  /// \brief Add a Key class which define how to a register free hook and
  /// how to get the current pthread identity.
  template <typename T>
  class ThreadKey
  {
  public:
    ThreadKey();
    ~ThreadKey();

    typedef pthread_t type;
    /// Return the unmutable identity of threads.
    static type current();
    /// Register the function \a free inside the hooks of the threads.
    void register_free_hook(boost::function1<void, type> free);
    /// Hook to manipulate thread data after the definition of a new value.
    void set_hook();
    /// internal function used to be called by thread delete hook.
    void cleanup();
  private:
    /// Key identity.
    pthread_key_t key;
    /// Free hook function.
    boost::function1<void, type> free_;
  };

  /// \brief Define specific thread pointers.
  template <typename T>
  struct ThreadSpecificPtr
    : SpecificPtr<T, ThreadKey<T> >
  {
  };

  namespace localdata
  {
    /// \brief Define Thread encapsulation type.
    struct Thread {};

    /// \brief Define types used by local data for thread encapsulation.
    template <typename T>
    struct Wrapper<T, Thread>
    {
      typedef T target;
      typedef T type;
      typedef ThreadSpecificPtr<type> container;
    };
  }

}

# include <libport/thread-data.hxx>

#endif
