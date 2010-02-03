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

  template <typename T>
  class SCHED_API CoroutineKey
  {
  public:
    CoroutineKey();
    ~CoroutineKey();

    typedef Coro* type;
    static type current();
    void register_free_hook(boost::function1<void, type> free);
    void set_hook();
  private:
    boost::function1<void, type> free_;
  };

  template <typename T>
  struct SCHED_API CoroutineSpecificPtr
    : ::libport::SpecificPtr<T, CoroutineKey<T> >
  {
  };
}

namespace libport
{
  namespace localdata
  {
    struct Coroutine {};

    template <typename T>
    struct Wrapper<T, Coroutine>
    {
      typedef T target;
      typedef T type;
      typedef ::sched::CoroutineSpecificPtr<type> container;
    };

    struct ThreadCoroutine {};

    template <typename T>
    struct Wrapper<T, ThreadCoroutine>
    {
      typedef T target;
      typedef libport::LocalData<T, Coroutine> type;
      typedef ThreadSpecificPtr<type> container;
    };
  }
}

# include <sched/coroutine-data.hxx>

#endif
