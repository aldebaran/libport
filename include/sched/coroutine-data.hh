/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SCHED_COROUTINE_DATA_HH
# define SCHED_COROUTINE_DATA_HH

# include <libport/local-data.hh>
# include <libport/specific-ptr.hh>
# include <libport/thread-data.hh>

# include <sched/coroutine.hh>
# include <sched/export.hh>

namespace sched
{
  /// \brief Add a Key class which define how to a register free hook and
  /// how to get the current coroutine identity.
  template <typename T>
  class CoroutineKey
  {
  public:
    CoroutineKey();
    ~CoroutineKey();

    typedef Coro* type;
    /// Return the unmutable identity of coroutines.
    static type current();
    /// Register the function \a free inside the hooks of the coroutines.
    void register_free_hook(boost::function1<void, type> free);
    /// Do nothing, but requiered for SpecificPtr \p Key parameter.
    void set_hook();
  private:
    /// Free hook function.
    boost::function1<void, type> free_;
  };

  /// \brief Define specific coroutine pointers.
  template <typename T>
  struct CoroutineSpecificPtr
    : ::libport::SpecificPtr<T, CoroutineKey<T> >
  {
  };
}

namespace libport
{
  namespace localdata
  {
    /// \brief Define Coroutine encapsulation type.
    struct Coroutine {};

    /// \brief Define types used by local data for coroutine encapsulation.
    template <typename T>
    struct Wrapper<T, Coroutine>
    {
      typedef T target;
      typedef T type;
      typedef ::sched::CoroutineSpecificPtr<type> container;
    };

    /// \brief Define Thread and Coroutine encapsulation type.  This
    /// encapsulation should be used when the number of thread is lower than
    /// the number of coroutine.
    struct ThreadCoroutine {};

    /// \brief Define types used by local data for thread and coroutine
    /// encapsulation.
    template <typename T>
    struct Wrapper<T, ThreadCoroutine>
    {
      typedef T target;
      typedef libport::LocalData<T, Coroutine> type;
      // The coroutine will take care of the destruction of LocalData, so
      // do nothing when the thread dies.
      typedef UnmanagedThreadSpecificPtr<type> container;
    };
  }
}

# include <sched/coroutine-data.hxx>

#endif
